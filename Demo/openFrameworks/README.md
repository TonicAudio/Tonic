### How to Install the openFrameworks demos (for OSX)

1) Copy each demo's folder to the openFrameworks "myApps" directory (path/to/openFrameworks/apps/myApps/

![copy demos](http://github.com/TonicAudio/TonicDocs/blob/master/screenshots/oF_install_instructions/copy_oF_demos.png?raw=true)

2) Each project contains a Project.xcconfig file. In this file, change TONIC_PATH to point to the `src/` directory of
the Tonic project, wherever it lives on your computer.

![tonic path](http://github.com/TonicAudio/TonicDocs/blob/master/screenshots/oF_install_instructions/set_tonic_path.png?raw=true)

3) Click on Tonic->TonicLib.xcodeproj within the Demo project (it should appear as a missing file). In the Inspector panel of Xcode, click the icon under
"Location" and locate lib/TonicLib.codeproj, within the Tonic directory.

![library file](http://github.com/TonicAudio/TonicDocs/blob/master/screenshots/oF_install_instructions/tonic_lib_loc.png?raw=true)
<br><br>
![library location](http://github.com/TonicAudio/TonicDocs/blob/master/screenshots/oF_install_instructions/find_tonic_lib.png?raw=true)

4) Follow the same steps in 3) to locate the "Demo Synths" directory if necessary.

5) Click the demo project file. Under "Targets", click the project target. Click the "Build Phases" tab. Expand "Target Dependencies" and click the plus icon to add "TonicLib-OSX"

6) Expand "Link Binary with Libraries" and click the plus icon to add "TonicLib-OSX"

![dependencies](http://github.com/TonicAudio/TonicDocs/blob/master/screenshots/oF_install_instructions/add_dependencies.png?raw=true)


The demos should now compile and run! 
