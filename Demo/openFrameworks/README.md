HOW TO INSTALL THESE DEMOS

1) Copy each demo's folder to (path_to_openframeworks)/apps/myApps/


2) Each project contains a Project.xcconfig file. In this file, change TONIC_PATH to point to the `src/` directory of
the Tonic project, wherever it lives on your computer.

3) Click on Tonic->TonicLib.xcodeproj within the Demo project (it should have red text, indicating Xcode couldn't find
	it). In the Inspector panel of Xcode, click the icon under
"Location" and locate lib/TonicLib.codeproj, within the Tonic directory.

The demos should now work!
