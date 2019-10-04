# To build AR apps with ofxARKit
1. Set team member in in Signing & Capabilites
2. Build Phases -> Compile Sources -> add Shaders.metal (which is under ofxARKit/src/lib)
3. openFrameworks/ofxiOs-Info.plist
    * Add: Privacy - Camera usage Description
4. addons/ofxARKit/src/lib/Shaders.metal
    * Type: Default - Metal Shader Source
    