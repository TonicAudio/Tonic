#ifndef ADAPTEST_BUF_H
#define ADAPTEST_BUF_H

#include <adaptest.h>

// a Testcase Base Class for Adaptest which compares Buffers of a given type and
// is able to write the buffers and a matching gnuplot script to the filesystem
// upon test failure.

// writing into files is disabled by default
#ifndef ADAPTEST_BUFWRITE_FILE
#define ADAPTEST_BUFWRITE_FILE 0
#endif // !ADAPTEST_BUFWRITE_FILE

#ifndef ADAPTEST_BUFWRITE_CSV_FILENAME_FORMAT
#define ADAPTEST_BUFWRITE_CSV_FILENAME_FORMAT "%s-%s.csv"
#endif // !ADAPTEST_BUFWRITE_CSV_FILENAME_FORMAT

#ifndef ADAPTEST_BUFWRITE_GNUPLOT_FILENAME_FORMAT
#define ADAPTEST_BUFWRITE_GNUPLOT_FILENAME_FORMAT "%s-%s.plt"
#endif

#ifndef ADAPTEST_BUFWRITE_HTML_FILENAME_FORMAT
#define ADAPTEST_BUFWRITE_HTML_FILENAME_FORMAT "%s-%s.html"
#endif // !ADAPTEST_BUFWRITE_HTML_FILENAME_FORMAT

#if ADAPTEST_BUFWRITE_FILE
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#endif //ADAPTEST_BUFWRITE_FILE

namespace ADAPTEST_NAMESPACE {

#if ADAPTEST_BUFWRITE_FILE
  template <class T>
  class BufferWriter {
  public:
    typedef T  value_type;
    typedef const T* pointer_type;

    struct Buffer {
      std::string name;
      pointer_type buf;
      size_t buflen;

      Buffer(std::string _name, pointer_type _buf, size_t _buflen)
      : name(_name)
      , buf(_buf)
      , buflen(_buflen)
      {}

    };

    typedef std::list<Buffer> BufferList;
    typedef typename BufferList::iterator BufferListIter;
    BufferList data;
    BufferList& getData() { return data; }
    std::ostream& msg;
    const int line;
    Testcase& testcase;

    virtual Result write() = 0;

    BufferWriter(
      std::ostream& _msg, const int _line, 
      class Testcase& _testcase)
    : msg(_msg)
    , line(_line)
    , testcase(_testcase)
    {}

    void add_buf(const T* buf, const size_t buflen, std::string name)
    {
      data.push_back(Buffer(name, buf, buflen));
    }

    Result error(std::string errmsg) {
      return testcase.error(msg, line, errmsg);
    }

    const char * getTestcaseName() {
      return testcase.getName().c_str();
    }

    const char * getTestsuiteName() {
      return testcase.getTestsuite().getName().c_str(); 
    }
  };

  //--------------------------------------------------------------------------

    template <class T>
    class CSVBufferWriter : public BufferWriter<T> {
      using typename BufferWriter<T>::Buffer;
      using typename BufferWriter<T>::BufferList;
      using typename BufferWriter<T>::BufferListIter;
      using BufferWriter<T>::getTestcaseName;
      using BufferWriter<T>::getTestsuiteName;
      using BufferWriter<T>::error;
      using BufferWriter<T>::write;
      using BufferWriter<T>::getData;
    public:
      CSVBufferWriter(
        std::ostream& _msg, const int _line, 
        class Testcase& _testcase)
      : BufferWriter<T>(_msg,_line, _testcase)
      {}

      virtual Result write_buffers(std::ostream& datafile)
      {
        // write the data columwise
        const size_t buflen = getData().begin()->buflen;
        for (int i = 0; i < buflen; ++i)
        {
          for (BufferListIter it = getData().begin(); it != getData().end(); ++it)
          {
            if (it != getData().begin()) datafile << ",";
            Buffer& data = *it;
            datafile << data.buf[i];
          }
          datafile << std::endl;
        }

        return OK;
      }

      virtual Result write_gnuplot(std::ostream& gnuplot, Formatted& filename)
      {
        gnuplot << "plot ";
        int bufidx = 0;
        for (BufferListIter i = getData().begin(); i != getData().end(); ++i)
        {
          if (i != getData().begin()) gnuplot << ", ";

          gnuplot 
            << "\"" << filename << "\" "
            << "using " << bufidx << " "
            << "title \"" << i->name << "\" "
            << "with lines";

          bufidx++;
        }

        gnuplot << std::endl;
        return OK;
      }

      virtual Result write_html(std::ostream& html, Formatted& filename)
      {
        html <<
          "<html><head>"
          "<script type=\"text/javascript\""
          "  src=\"dygraph.js\"></script>"
          "  <style type=\"text/css\">body, div {padding: 0;margin: 0;}</style>"
          "</head><body>"
          "<div id=\"graphdiv\"></div>"
          "<script type=\"text/javascript\">"
          "  function getData() {return [";

        int bufidx = 0;
        BufferListIter begin = getData().begin();
        for (BufferListIter i = begin; i != getData().end(); ++i)
        {
          if (i != begin) html << ", ";

          // write the data columwise
          const size_t buflen = begin->buflen;
          for (int i = 0; i < buflen; ++i)
          {
            if (i > 0) html << ",";
            html << "[ " << i << ", ";
            for (BufferListIter it = begin; it != getData().end(); ++it)
            {
              if (it != begin) html << ",";
              Buffer& data = *it;
              html << data.buf[i];
            }
            html << "] " << std::endl;
          }

          bufidx++;
        }

        html <<
          "  ];}"
          "  (function() {"
          "  var w = window, d = document, e = d.documentElement, "
          "      g = d.getElementsByTagName('body')[0],"
          "      width = w.innerWidth || e.clientWidth || g.clientWidth,"
          "      height = w.innerHeight|| e.clientHeight|| g.clientHeight;"
          "  g = new Dygraph(document.getElementById(\"graphdiv\"), getData, {"
          "      width: width,"
          "      height: height,"
          "      labels: [ \"Index\", ";

        for (BufferListIter it = getData().begin(); it != getData().end(); ++it)
        {
          Buffer& data = *it;
          if (it != getData().begin()) html << ", ";
          html << "\"" << data.name << "\"";
        }

        html <<
          " ],"
          "    });})();</script></body></html>";

        return OK;
      }      


      Result write() {
        Formatted filename(ADAPTEST_BUFWRITE_CSV_FILENAME_FORMAT,
                           getTestsuiteName(), getTestcaseName());
        std::fstream datafile(filename.ptr(), std::ios::out);
        
        if (!datafile.good()) {
          return error(Formatted("could not open %s", filename.ptr()));
        }

        Formatted gnuplot_filename(ADAPTEST_BUFWRITE_GNUPLOT_FILENAME_FORMAT, 
                                   getTestsuiteName(), getTestcaseName());        
        std::fstream gnuplot_file(gnuplot_filename.ptr(), std::ios::out);

        if (!gnuplot_file.good()) {
          return error(Formatted("could not open %s", gnuplot_filename.ptr()));
        }

        Formatted html_filename(ADAPTEST_BUFWRITE_HTML_FILENAME_FORMAT, 
                                   getTestsuiteName(), getTestcaseName());        
        std::fstream html_file(html_filename.ptr(), std::ios::out);

        if (!html_file.good()) {
          return error(Formatted("could not open %s", html_filename.ptr()));
        }

        write_buffers(datafile);
        write_gnuplot(gnuplot_file, filename);
        write_html(html_file, filename);

        return OK;
      }
    };


#endif // ADAPTEST_BUFWRITE_FILE  

  // ======================================================================== 

  template <template <class> class WriterPolicy>
  class BufferTestcase : public virtual Testcase {
  public:
    // make test_eq overridable
    using Testcase::test_eq;

    template <class T>
    Result test_buf( 
      std::ostream& msg, const int line, 
      const size_t buflen, const T* buf, const T* expected, std::string name)
    {
      Result res = OK;
      for (size_t i=0; i<buflen; i++){
        res = test_eq(msg, line, expected[i], buf[i], 
                      Formatted("%s[%i]", name.c_str(), i));
        if (res != OK ) break;
      }

      #if ADAPTEST_BUFWRITE_FILE
        if (res != OK) {
          WriterPolicy<T> writer(msg, line, *this);
          writer.add_buf(buf, buflen, name);
          writer.add_buf(expected, buflen, Formatted("%s-expected", name.c_str()));
          writer.write();
        }
      #endif // ADAPTEST_BUFWRITE_FILE

      return res;
    }

    template <class T>
    Result test_buf( 
      std::ostream& msg, const int line, 
      const T* buf, const size_t buflen, const T expected, std::string name)
    {
      T expectedBuf[buflen];
      for (size_t i=0; i<buflen; i++){
        expectedBuf[i] = expected;
      }
      return test_buf(msg, line, buflen, buf, expectedBuf, 
                      Formatted("%s-expected", name.c_str()));
    }    

    template <class T>
    Result test_buf( 
      std::ostream& msg, const int line, 
      const size_t buflen, const T* buf, const size_t offset, const size_t step, 
      const T* expected, std::string name)
    {
      Result res = OK;
      for (size_t i=0; i<buflen; i++){
        res = test_eq(msg, line, expected[i], buf[i], 
                      Formatted("%s[%i]", name.c_str(), i));
        if (res != OK ) break;
      }

      #if ADAPTEST_BUFWRITE_FILE
        if (res != OK) {
          WriterPolicy<T> writer(msg, line, *this);
          writer.add_buf(buf, buflen, name);
          writer.add_buf(expected, buflen, Formatted("%s-expected", name.c_str()));
          writer.write();
        }
      #endif // ADAPTEST_BUFWRITE_FILE

      return res;
    }

    template <class T, size_t N>
    Result test_buf( 
      std::ostream& msg, const int line, 
      const T (&buf)[N], const T expected, std::string name)
    {
      return test_buf(msg, line, &buf[0], N, expected, name);
    }

    template <class T, size_t N>
    Result test_buf( 
      std::ostream& msg, const int line, 
      const T (&buf)[N], const T (&expected)[N], std::string name)
    {
      return test_buf(msg, line, N, &buf[0], &expected[0], name);
    }    

  };
} // namespace ADAPTEST_NAMESPACE

#endif //ADAPTEST_BUF_H