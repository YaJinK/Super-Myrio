# Super-Myrio
## Program Finished Date: 2016/9/29
    A little mario game
    I made this little program to review my DirectX 9.0c knowlage, not for making a good game.
    I did not have a perfect plan before coding, so this program has some problems. I will show you later.
    It is a little game, but the spirit moved by force. Gravity, friction, acting force and reacting force.
    I updated this little mess program, because i want to make more friends with game programmers.
## Some tips of Super Myrio:
###    In the menu:
        W/↑: up
        S/↓: down
        J/Enter: ok
        K/ESC: back
###    In the game:
        A: left
        D: right
        S: squat -- you must be a big guy
        J: move quickly
           PRATTLE: I did not add fire balls, so if you want kill monster, you can only tread on their head. 
                    Fucking weak myrio!!
        K: jump -- press longer, jump higher.
           NOTICE: If you want to jump into a narrow place, you'd better jump to a same height as the place.
###     Three styles of map:
     Generate randomly. I will show you every style in Preview.
     
###     Three endings:
     Find them baby!
        PRATTLE: What's the fuck? GALGAME? Is there a princess suck Myrio's **?
                 ME:....
## Some bugs -- VERY IMPORTANT!
### 1. Do you want to close window? HA.. NOWAY!
       Some times, you will find that you can not close the game window. 
       Cross button, ESC or Exit selection in menu, whatever is invalid.
       But there is a way to handle it. The task manager!
### 2. Hey! Myrio! Where are you?
       When you squat and jump, the myrio may disapear when he arrive floor. 
       The reason is the mistake of texture manager. 
       It's easy to fix it, but i didn't do that. So what? Come here and hit my face!HAHAHA.
### 3. Through the wall!
       If you are lucky, you can jump into the wall! But you can not move like Mario on FC.
## Principle -- MOST IMPORTANT
### 1. Separate the resource and data
        I put picture and the game data to a same class. 
        So when i create a new object, the pictrue will be loaded again.
        It waste memory.
### 2. One class should not has to many functions
        In map class, i set a function to determine whether two rectangles cross or not.
        It can not be useed by other program. It's not right.
### 3. Use a virtual destructor
## Preview
![image](https://raw.githubusercontent.com/YaJinK/Super-Myrio/master/preview/1.JPG)
![image](https://raw.githubusercontent.com/YaJinK/Super-Myrio/master/preview/2.JPG)
![image](https://raw.githubusercontent.com/YaJinK/Super-Myrio/master/preview/3.JPG)
![image](https://raw.githubusercontent.com/YaJinK/Super-Myrio/master/preview/4.JPG)
## Next game
    Counter-Strike (Local Edition)
