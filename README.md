# final_project
1-2 final project
## Download and install
* You need to git clone this project with --recursive option.
      `git clone https://github.com/zylee3/final_project.git --recursive`
* You need to build the tomlc99 with "make" under tomlc99 subdirectory of your clone directory.
      `cd tomlc99`
      `make`
* You need to deploy the header and lib file with "sudo make install".
      `sudo make install`

## Run
* You need to build the demo_script with "make" under demo subdirectory of your clone directory.
      `cd demo`
      `make`
* To run demo_script, you need to specify location of script.tmol.
      `./script_player script/script.toml`

## Script and picture
* For script and picture-related questions, please see script/readme_script.txt

## How to use your script
      ./script_player [your_script_file]
      `./script_player your_script.toml`

## Some known issues
      Between two events that change the background  e.g. `[event.change_first_story_one] scene = "scene.first" dialogue = "dialogue.story_first_one"`.
      If you add more than six character , it won't show on the screen.