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

They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberries for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything.

Alice: OMG! The strawberries are so expensive.
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
1) strawberries
2) apples

3. New toml file.

Baking.toml

4. Add name and author for the story.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

5. Add all the scenes, characters, and items in the toml. Use the format [scene.place], the [character.name], or [item.thing] to name them. The background of the place stores in the folder "bg". The tachie of the character stores in the folder "cc". The avatar of the character and image of the item store in the folder "item". Those parameters should store the string before ".png".

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = "home"

[scene.supermarket]
name = "Supermarket"
background = "supermarket"

[character.alice]
name = "Alice"
avatar = "girl_avatar"
tachie = "girl"

[character.bob]
name = "Bob"
avatar = "boy_avatar"
tachie = "boy"

[item.strawberries]
name = "strawberries"
image = "strawberries"

[item.apples]
name = "apples"
image = "apples"

6. Add dialogue. There are two types of them, story and conversation. Use the format [dialogue.type.place_chapter.section] to name them.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = "home"

[scene.supermarket]
name = "Supermarket"
background = "supermarket"

[character.alice]
name = "Alice"
avatar = "girl_avatar"
tachie = "girl"

[character.bob]
name = "Bob"
avatar = "boy_avatar"
tachie = "boy"

[item.strawberries]
name = "strawberries"
image = "strawberries"

[item.apples]
name = "apples"
image = "apples"

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[[dialogue.conversation.home]]
[dialogue.conversation.home.one]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

[dialogue.conversation.home.two]
character = "Bob"
text = "How about baking the cake on our own?"

[dialogue.conversation.home.three]
character = "Alice"
text = "That's a great idea."

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberries for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[[dialogue.conversation.supermarket_one]]
[dialogue.conversation.supermarket_one.one]
character = "Alice"
text = "OMG! The strawberries are so expensive."

[dialogue.conversation.supermarket_one.two]
character = "Bob"
text = "Can we afford them?"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[[dialogue.conversation.supermarket_two]]
[dialogue.conversation.supermarket_two.one]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

[dialogue.conversation.supermarket_two.two]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

[dialogue.conversation.supermarket_two.three]
character = "Bob"
text = "I see. Should we buy them?"

[dialogue.conversation.supermarket_two.four]
character = "Alice"
text = "I'd like Japanese apples."

[dialogue.conversation.supermarket_two.five]
character = "Bob"
text = "OK."

7. When changing scene, adding event. The event stores the next scene and the next dialogue. Use the format [event.change_place_type_chapter] to name them.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = "home"

[scene.supermarket]
name = "Supermarket"
background = "supermarket"

[character.alice]
name = "Alice"
avatar = "girl_avatar"
tachie = "girl"

[character.bob]
name = "Bob"
avatar = "boy_avatar"
tachie = "boy"

[item.strawberries]
name = "strawberries"
image = "strawberries"

[item.apples]
name = "apples"
image = "apples"

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[[dialogue.conversation.home]]
[dialogue.conversation.home.one]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

[dialogue.conversation.home.two]
character = "Bob"
text = "How about baking the cake on our own?"

[dialogue.conversation.home.three]
character = "Alice"
text = "That's a great idea."

[event.change_supermarket_story_one]
scene = "scene.supermarket"
dialogue = "dialogue.story.supermarket_one"

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberries for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[[dialogue.conversation.supermarket_one]]
[dialogue.conversation.supermarket_one.one]
character = "Alice"
text = "OMG! The strawberries are so expensive."

[dialogue.conversation.supermarket_one.two]
character = "Bob"
text = "Can we afford them?"

[event.change_supermarket_story_two]
scene = "scene.supermarket"
dialogue = "dialogue.story.supermarket_two"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[[dialogue.conversation.supermarket_two]]
[dialogue.conversation.supermarket_two.one]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

[dialogue.conversation.supermarket_two.two]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

[dialogue.conversation.supermarket_two.three]
character = "Bob"
text = "I see. Should we buy them?"

[dialogue.conversation.supermarket_two.four]
character = "Alice"
text = "I'd like Japanese apples."

[dialogue.conversation.supermarket_two.five]
character = "Bob"
text = "OK."

8. When changing to next dialogue or event, adding next.

Baking.toml:
name = "A Piece Of Cake"
author = "zylee"

[scene.home]
name = "Home"
background = "home"

[scene.supermarket]
name = "Supermarket"
background = "supermarket"

[character.alice]
name = "Alice"
avatar = "girl_avatar"
tachie = "girl"

[character.bob]
name = "Bob"
avatar = "boy_avatar"
tachie = "boy"

[item.strawberries]
name = "strawberries"
image = "strawberries"

[item.apples]
name = "apples"
image = "apples"

[dialogue.story.home]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."
next = "dialogue.conversation.home"

[[dialogue.conversation.home]]
[dialogue.conversation.home.one]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."
next = "dialogue.conversation.home.two"

[dialogue.conversation.home.two]
character = "Bob"
text = "How about baking the cake on our own?"
next = "dialogue.conversation.home.three"

[dialogue.conversation.home.three]
character = "Alice"
text = "That's a great idea."
next = "event.change_supermarket_story_one"

[event.change_supermarket_story_one]
scene = "scene.supermarket"
dialogue = "dialogue.story.supermarket_one"

[dialogue.story.supermarket_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberries for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."
next = "dialogue.conversation.supermarket_one"

[[dialogue.conversation.supermarket_one]]
[dialogue.conversation.supermarket_one.one]
character = "Alice"
text = "OMG! The strawberries are so expensive."
next = "dialogue.conversation.supermarket_one.two"

[dialogue.conversation.supermarket_one.two]
character = "Bob"
text = "Can we afford them?"
next = "event.change_supermarket_story_two"

[event.change_supermarket_story_two]
scene = "scene.supermarket"
dialogue = "dialogue.story.supermarket_two"

[dialogue.story.supermarket_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."
next = "dialogue.conversation.supermarket_two"

[[dialogue.conversation.supermarket_two]]
[dialogue.conversation.supermarket_two.one]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"
next = "dialogue.conversation.supermarket_two.two"

[dialogue.conversation.supermarket_two.two]
character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."
nect = "dialogue.conversation.supermarket_two.three"

[dialogue.conversation.supermarket_two.three]
character = "Bob"
text = "I see. Should we buy them?"
next = "dialogue.conversation.supermarket_two.four"

[dialogue.conversation.supermarket_two.four]
character = "Alice"
text = "I'd like Japanese apples."
next = "dialogue.conversation.supermarket_two.five"

[dialogue.conversation.supermarket_two.five]
character = "Bob"
text = "OK."

Explaination of the structures in parsing.h:

enum StructId:
 To identify the struct.

Container:
 For table and array.

IdParentChild:
 To identify the first child and the others are linked by the _next field.
 (It is just a header of the linked list for children.)

FinalProject:
 To store the title and author of this final project.

Scene:
 _id: to identify the struct as scene.
 _parent: the parent of this structure.
 _child: of no use, to make all structure consistent under cast.
 _next: the "siblings" of this structure. (sub-linked list)
 name: the name of the scene.
 backgound: the storage location of the background image.

Character:
 _id: to identify the struct as scene.
 _parent: the parent of this structure.
 _child: of no use, to make all structure consistent under cast.
 _next: the "siblings" of this structure. (sub-linked list)
 name: the name of the character.
 avatar: the storage location of the avatar image.
 tachie: the storage location of the tachie image.
 favor_cat: the favorability of the cats.
 favor_dog: the favorability of the dogs. 

Item:
 _id: to identify the struct as scene.
 _parent: the parent of this structure.
 _child: of no use, to make all structure consistent under cast.
 _next: the "siblings" of this structure. (sub-linked list)
 name: the name of the item.
 image: the storage location of the image

Event:
 _id: to identify the struct as scene.
 _parent: the parent of this structure.
 _child: of no use, to make all structure consistent under cast.
 _next: the "siblings" of this structure. (sub-linked list)
 scene: to show the scene that should be displayed.
 dialogue: go to the specified dialogue.

Dialogue:
 _id: to identify the struct as scene.
 _parent: the parent of this structure.
 _child: of no use, to make all structure consistent under cast.
 _next: the "siblings" of this structure. (sub-linked list)
 character: the speaking character.
 text: the story context.
 next: go to the specified dialogue or event.

Find: To find how many structures of the specified name in the toml.
 capacity: the maximum of the structures that can be placed.
 count: the counts of structures found, negative for error.
 ppvMatches: the pointer of the found structures.

FullNameRecord: To find the structure of the specified name in a quicker way. (The structure is unique.)
 fullName: the specified name.
 order: the order in the toml.
 record: the pointer of the found structure.
