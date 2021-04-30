# Winamp-Based Visualizer
A Winamp/foobar2000 spectrum visualizer for CIS 367.  All graphics are done using OpenGL and libpng.  This plugin is completely compatible with Winamp or any Winamp emulators such as Shpeck although it can be a bit buggy on the later.  

![Visualizer](https://imgur.com/pysPVDk.png)

#### Features
- High resolution spectrum
- Peak cap markers
- Background flash beat detection

## How to use
You can check the releases page to download visualizer.zip.  If you intent to use foobar2000, make sure to install Shpeck found [here](https://www.foobar2000.org/components/view/foo_vis_shpeck).  More foobar2000 installation instructions can be found [here](https://wiki.hydrogenaud.io/index.php?title=Foobar2000:How_to_install_a_component).  

NOTE: Shpeck is not a finished product and can be buggy and crash foobar2000 occasionally.  For an easier experience use Winamp.

#### Foobar2000 (buggy)
- Navigate to your foobar2000 folder and there should be a folder called Plugin.  
- Extract visualizer.zip into the Plugin folder and ensure ```vis_visualizer.dll``` and ```overlay.png``` were extracted.
- There are multiple ways to start the visualization:
  - Enter foobar2000 preferences via the file dropdown, navigate to the Visualizations tab and click Shpeck.  If there are no available plugins click refresh.  Select Spectrum Visualizer in the text area and click the Start/Stop button.
  - Under the view dropdown menu select Visualizations -> Shpeck - Start 'Spectrum Visualizer/Spectrum Visualizer'

#### Winamp
- Navigate to your Winamp folder and find a folder called Plugins
- Extract visualizer.zip into the Plugin folder and ensure ```vis_visualizer.dll``` and ```overlay.png``` were extracted.
- In Winamp, right-click on the visualization tab.  Navigate to Visualizations -> Select Plugin...
- Click on Spectrum Visualizer then click the Start button and the visualizer should appear in the visualization window.

Enjoy!

## How to compile
#### Requirements
- Visual Studio 2015 or later
- [Winamp SDK 5+](http://wiki.winamp.com/wiki/SDK_Contents)
- OpenGL (Usually included with windows)
- (included) [libpng](http://www.libpng.org/pub/png/libpng.html)

#### Setup
Clone the repository and open the provided project file.  There are a few things in the project configuration that might need changing.  Navigate to the project property page by right clicking the visualizer project and selecting properties.
- In the VC++ Directories menu, make sure that the Winamp SDK include directory is correct or change it to the path of your Winamp SDK install.
- In the Linker -> Input menu, ensure that OpenGL32.lib and glu32.lib or their equivalents are present.
- (Optional) To automatically copy the compiled files to the Plugin folders a copy command can be added in Build Events -> Post-Build Event.  Copy,
  ```
  copy /y "$(TargetDir)$(ProjectName).dll" "<plugin_folder>\vis_visualizer.dll"
  copy /y "$(ProjectDir)assets\overlay.png" "<plugin_folder>\overlay.png"
  ```
  and replace ```<plugin_folder>``` with the path to the respective plugin folder on your system.

#### Building
Select Release and x86 and build the solution.  If the copy automation steps were added to your project from the previous section and the build was successful, the plugin should be ready to go.  Otherwise copy the compiled DLL to your plugin folder and rename it ```vis_visualizer.dll``` (add the ```vis_``` prefix for Winamp categorization).  Also copy the ```overlay.png``` file from the asset folder to the same directory.  Check the [How to use](#how-to-use) section for further instructions.

## Acknowledgements
This project helped alot with learning Winamp and OpenGL setup: https://github.com/icebreaker/tiny3d
