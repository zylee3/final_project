# final_project
1-2 final project
## Download and install
* You need to git clone this project with --recursive option.
      `git clone https://github.com/zylee3/final_project.git --recursive`

## Run
* You need to build the  with `make` under your clone directory.
* To run script_player with script, you need to specify location of tmol file, use
  `./script_player [file]`
* If you want to try our script, use
      `./script_player script/script.toml`
* You can also use your script, use `./script_player "your_script".toml`
* Don't forget to add the relative path between script_player and script file.

## Script and picture
* All picture must be .png format.
* Picture's file name shouldn't have space. `Red ball.png` is "not" OK. `Red_ball.png` is OK.
* For more script and picture-related questions, please see script/readme_script.txt


## Some known issues
* Between two events that change the background,
* if you add more than six character , 7th and following character won't show on the screen.
* Example of an event.
```
[event.change_first_story_one]
scene = "scene.first"
dialogue = "dialogue.story_first_one"
```
