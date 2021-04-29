# foobar2k-visualizer
A foobar2000/Winamap spectrum visualizer for CIS 367.  All graphics are done using OpenGL and libpng.  This plugin is completely compatible with Winamp or any Winamp emulators such as Shpeck although it can be a bit buggy.  

## How to use
You can check the releases page to download visualizer.zip.  If you intent to use foobar2000, make sure to install Shpeck found [here](https://www.foobar2000.org/components/view/foo_vis_shpeck).  More installation instructions can be found [here](https://wiki.hydrogenaud.io/index.php?title=Foobar2000:How_to_install_a_component).

#### Foobar2000
- Navigate to your foobar2000 folder and there should be a folder called Plugin.  
- Extract visualizer.zip into the Plugin folder and ensure ```vis_visualizer.dll``` and ```overlay.png``` were extracted.
- There are multiple ways to start the visualization:
  - Enter foobar2000 preferences via the file dropdown, navigate to the Visualizations tab and click Shpeck.  If there are no available plugins click refresh.  Select Spectrum Visualizer in the text area and click the Start/Stop button.

This project helped alot with learning Winamp and OpenGL setup: https://github.com/icebreaker/tiny3d
