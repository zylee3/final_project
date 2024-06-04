The player is a member of guild. The story is about the adventure story of a party of four. There are three encounters, the first one is a fight with two Malayan night herons, the second one is a fight with four squirrels, the last one is a fight with a Dyson vacuum cleaner.
At the end of the story, there are three things found in the room. Player will choose different items favor different pets, affecting the ending.

Script file format: TOML
Script file name: script.toml
storage folder: script

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

[dialogue.home_story]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[dialogue.home_conversation]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

character = "Bob"
text = "How about baking the cake on our own?"

character = "Alice"
text = "That's a great idea."

[dialogue.supermarket_story_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[dialogue.supermarket_conversation_one]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

character = "Bob"
text = "Can we afford them?"

[dialogue.supermarket_story_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[dialogue.supermarket_conversation_one]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

character = "Bob"
text = "I see. Should we buy them?"

character = "Alice"
text = "I'd like Japanese apples."

character = "Bob"
text = "OK."

7. When changing to different scene, add "event".

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

[dialogue.home_story]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

[dialogue.home_conversation]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

character = "Bob"
text = "How about baking the cake on our own?"

character = "Alice"
text = "That's a great idea."

event = "change"

[event.change]
scene = "supermarket"
dialogue = "supermarket_story_one"

[dialogue.supermarket_story_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

[dialogue.supermarket_conversation_one]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

character = "Bob"
text = "Can we afford them?"

[dialogue.supermarket_story_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

[dialogue.supermarket_conversation_one]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

character = "Bob"
text = "I see. Should we buy them?"

character = "Alice"
text = "No. I'd like apples from Japan."

character = "Bob"
text = "OK."

8. When moving on to the next dialogue, add "next".

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

[dialogue.home_story]
text = "There were two close friends, Alice and Bob. One day, they decide to bake a cake."

next = "dialogue.home_conversation"

[dialogue.home_conversation]
character = "Alice"
text = "I want to eat strawberry cake, but it is too expensive."

character = "Bob"
text = "How about baking the cake on our own?"

character = "Alice"
text = "That's a great idea."

event = "change"

[event.change]
scene = "supermarket"
dialogue = "supermarket_story_one"

[dialogue.supermarket_story_one]
text = "They find a recipe. The next step is to buy all the ingredients needed. They have to buy a bottle of strawberry yogurt, 150 grams of cream, 10 grams of gelatine powder, and a box of strawberrys for mousse. For the bottom, they need to buy 80 grams of cookies, and unsalted butter. Fortunately, the supermaket a cross the street sells everything."

next = "dialogue.supermarket_conversation_one"

[dialogue.supermarket_conversation_one]
character = "Alice"
text = "OMG! The strawberrys are so expensive."

character = "Bob"
text = "Can we afford them?"

next = "dialogue.supermarket_story_two"

[dialogue.supermarket_story_two]
text = "After they find out that they don't have the budget for the cake, they decide to make apple pie instead of strawberry cake."

next = "dialogue.supermarket_conversation_two"

[dialogue.supermarket_conversation_two]
character = "Bob"
text = "Here are apples! Why is much cheaper then before?"

character = "Alice"
text = "These are from Chile. We used to buy apples from Japan."

character = "Bob"
text = "I see. Should we buy them?"

character = "Alice"
text = "No. I'd like apples from Japan."

character = "Bob"
text = "OK."