The player is a member of guild. The story is about the adventure story of a party of four. There are three encounters, the first one is a fight with two Malayan night herons, the second one is a fight with four squirrels, the last one is a fight with a Dyson vacuum cleaner.
At the end of the story, there are three things found in the room. Player will choose different items favor different pets, affecting the ending.

Script file format: TOML
Script file name: script.toml
storage folder: final_project/script

How to convert a script into a toml file:
1. Write a script, the script below is an example.

Baking.txt:
There were two close friends, Alice and Bob. One day, they decide to bake a cake.

Alice: I want to eat strawberry cake, but it is too expensive.
Bob: How about baking the cake on our own?
Alice: That's a great idea.

They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything.

Alice: OMG! The strawberrys are so expensive.
Bob: Can we afford them?

After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake.

Bob: Here are apples! Why is much cheaper then before?
Alice: These are from Chile. We used to buy apples from Japan.
Bob: I see. Should we buy them?
Alice: I'd like Japanese apples.
Bob: OK.

2. List all the scenes, characters, and items in the script.

scenes:
1) Home
2) Supermarket

characters:
1) Alice
2) Bob

items:
1) strawberrys
2) apples

3. New toml file.

Baking.toml

4. Add name and author for the story.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

5. Add all the scenes, characters, and items in the toml. 

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = ""

[scene.supermarket]
name = "Supermarket"
background = ""

[character.alice]
name = "Alice"
avatar = ""
tachie = ""

[character.bob]
name = "Bob"
avatar = ""
tachie = ""

[item.strawberrys]
name = "strawberrys"
tachie = ""

[item.apples]
name = "apples"
tachie = ""

6. Add dialogue.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = ""

[scene.supermarket]
name = "Supermarket"
background = ""

[character.alice]
name = "Alice"
avatar = ""
tachie = ""

[character.bob]
name = "Bob"
avatar = ""
tachie = ""

[item.strawberrys]
name = "strawberrys"
tachie = ""

[item.apples]
name = "apples"
tachie = ""

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[dialogue.conversation.home]
[[dialogue.conversation.home.one]]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

[[dialogue.conversation.home.two]]
character = "Bob"
text = "How about baking the cake on our own?"

[[dialogue.conversation.home.three]]
character = "Alice"
text = "That's a great idea."

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[dialogue.conversation.supermarket_one]
[[dialogue.conversation.supermarket_one.one]]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

[[dialogue.conversation.supermarket_one.two]]
character = "Bob"
text = "Can we afford them?"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[dialogue.conversation.supermarket_two]
[[dialogue.conversation.supermarket_two.one]]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

[[dialogue.conversation.supermarket_two.two]]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

[[dialogue.conversation.supermarket_two.three]]
character = "Bob"
text = "I see. Should we buy them?"

[[dialogue.conversation.supermarket_two.four]]
character = "Alice"
text = "I'd like Japanese apples."

[[dialogue.conversation.supermarket_two.five]]
character = "Bob"
text = "OK."

7. When changing to different scene, add "event.change_xxx" and set the scene and dialogue. For instance, if changing to supermarket, "event_change_supermarket".

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = ""

[scene.supermarket]
name = "Supermarket"
background = ""

[character.alice]
name = "Alice"
avatar = ""
tachie = ""

[character.bob]
name = "Bob"
avatar = ""
tachie = ""

[item.strawberrys]
name = "strawberrys"
tachie = ""

[item.apples]
name = "apples"
tachie = ""

[event.change_home]
scene = "scene.home"
dialogue = "dialogue.story.home"

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[dialogue.conversation.home]
[[dialogue.conversation.home.one]]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

[[dialogue.conversation.home.two]]
character = "Bob"
text = "How about baking the cake on our own?"

[[dialogue.conversation.home.three]]
character = "Alice"
text = "That's a great idea."

event = "event.change_supermarket"

[event.change_supermarket]
scene = "scene.home"
dialogue = "dialogue.story.supermarket_one"

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[dialogue.conversation.supermarket_one]
[[dialogue.conversation.supermarket_one.one]]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

[[dialogue.conversation.supermarket_one.two]]
character = "Bob"
text = "Can we afford them?"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[dialogue.conversation.supermarket_two]
[[dialogue.conversation.supermarket_two.one]]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

[[dialogue.conversation.supermarket_two.two]]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

[[dialogue.conversation.supermarket_two.three]]
character = "Bob"
text = "I see. Should we buy them?"

[[dialogue.conversation.supermarket_two.four]]
character = "Alice"
text = "I'd like Japanese apples."

[[dialogue.conversation.supermarket_two.five]]
character = "Bob"
text = "OK."

9. When moving on to the next dialogue or dialogue sub-array, add "next".

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = ""

[scene.supermarket]
name = "Supermarket"
background = ""

[character.alice]
name = "Alice"
avatar = ""
tachie = ""

[character.bob]
name = "Bob"
avatar = ""
tachie = ""

[item.strawberrys]
name = "strawberrys"
tachie = ""

[item.apples]
name = "apples"
tachie = ""

[event.change_home]
scene = "scene.home"
dialogue = "dialogue.story.home"

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

next = "dialogue.conversation.home"

[dialogue.conversation.home]
[[dialogue.conversation.home.one]]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

next = "dialogue.conversation.home.two"

[[dialogue.conversation.home.two]]
character = "Bob"
text = "How about baking the cake on our own?"

next = "dialogue.conversation.home.three"

[[dialogue.conversation.home.three]]
character = "Alice"
text = "That's a great idea."

event = "event.change_supermarket"

[event.change_supermarket]
scene = "scene.home"
dialogue = "dialogue.story.supermarket_one"

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

next = "dialogue.conversation.supermarket_one"

[dialogue.conversation.supermarket_one]
[[dialogue.conversation.supermarket_one.one]]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

next = "dialogue.conversation.supermarket_one.two"

[[dialogue.conversation.supermarket_one.two]]
character = "Bob"
text = "Can we afford them?"

next = "dialogue.story.supermarket_two"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

next = "dialogue.conversation.supermarket_two"

[dialogue.conversation.supermarket_two]
[[dialogue.conversation.supermarket_two.one]]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

next = "dialogue.conversation.supermarket_two.two"

[[dialogue.conversation.supermarket_two.two]]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

next = "dialogue.conversation.supermarket_two.three"

[[dialogue.conversation.supermarket_two.three]]
character = "Bob"
text = "I see. Should we buy them?"

next = "dialogue.conversation.supermarket_two.four"

[[dialogue.conversation.supermarket_two.four]]
character = "Alice"
text = "I'd like Japanese apples."

next = "dialogue.conversation.supermarket_two.five"

[[dialogue.conversation.supermarket_two.five]]
character = "Bob"
text = "OK."

Explaination of the structures in parsing.h:
Scene:
 name: the name of the scene
 backgound: the storage location of the background image

Character:
 name: the name of the character
 avatar: the storage location of the avatar image
 tachie: the storage location of the tachie image

Item:
 name: the name of the item
 image: the storage location of the image

Favor:
 name: the character name of the favorability owner
 favor_cats: the favorability toward cats
 favor_dogs: the favorability toward dogs

DialogueStory:
 name: the name of the story
 text: the story context
 next: next dialogue
 event: change background

DialogueConversation:
 name: the name of the story
 character: the speaking character
 text: the story context
 next: next dialogue
 event: change background
