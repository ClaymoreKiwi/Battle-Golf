## Contact

*Bryce Tingle - bryce.tingle@mail.bcu.ac.uk
*Student ID: 17140531
*
<!-- ABOUT THE PROJECT -->
#  Battle Golf

A fun sports survival game where you are playing a game of golf after hours. The staff are actively trying to remove you from the course but you're determine
to get the lowest par!

Although, you're playing with a twist. Every time you hit one of the staff with your club you have decided to add 5 shots to your score.
and when you throw any spare golf balls at the staff you gain 1 extra shot.

Dont run out of Stamina now, else you wont make it to the end of the course and you will lose!

<img width="399" alt="BG" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/2335271e-000d-4d49-b170-bb87a5ebc1e8">
<img width="597" alt="Screenshot 2023-12-09 154835" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/5d3bfeb1-ce59-48d1-a76f-3c56e1bd4ed1">

Features:
* Use your golf club to play golf and hit the ball ⛳
* Throw your golf club to enemies to knock them out - has to be collected to continue hitting the balls 🏌️‍♂️
* Collect spare golf balls to throw at the staff
* If they catch you you should shake them off before your stamina runs out
* Fight your way through rough grass and bunkers that effect the players movement
* Try get the lowest score you can! :moneybag:

## How to Play
* W,A,S,D to move around
* left click to hit ball
* right click to thow club
* scroll wheel (up or down) to change ammo type
* when in range of the ball - click and drag the mouse in the opposite direction to shoot

## Dependencies
* SDL library
* SDL Mixer
* SDL Image
* SDL TTF

## Target platforms
Support Windows x64

Instructions to build on Windows platforms:
1) Load .sln solution with Visual Studio;
2) Select Release configuration, and x64 Architecture;
3) Compile and run the project.

## TODO

* Video playing
* loading screen

## COMPLETE
* A* pathfinding for enemies
* Optimise
* Hitting the golf ball
* Particles
* Collisions - rect & circle
* Loading external files - binary and txt
* Enemy spawn in
* Stamina bar
* Shooting
* Menu / Game Music
* Button SFX
* Main Menu
* lose and win screens
* Tiled Map
* Spritesheet Animations

## code section profiling / memory management
Main Menu loop:<br />
20 - 40ms first run<br />
3-5ms every loop<br/>
* usage of pointers and passing by refrence where possible to reduce copying into functions
* trying to minimise branches where possible to minimise decisions
<br/>

Gameloop = 80 - 90ms every loop (deliberate delay)<br />
gameloop without this takes 3-5ms (desktop)<br />
Main Menu takes 3-9ms (desktop)<br />
Lose Screen takes 5-12ms (laptop)<br />
Win Screen takes 13 -17ms (laptop)<br/>
Update loop for everything  1-3ms (desktop)<br/>
<img width="563" alt="Screenshot 2023-12-15 235334" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/9cda9c87-a1bb-4ca9-89fa-89784a19c056"><br/>
draw loop with 80ms delay 80 - 90ms<br> (biggest cost in the program)<br/>
* improved with textures and GPU acceleration
* this was also done for the particles with a 1 - 3ms loop time <br>
<img width="515" alt="Screenshot 2023-12-15 235519" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/25499a21-9eca-439c-8cc4-454bb6c98a10"><br/>
<img width="712" alt="Screenshot 2023-12-16 000354" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/4ecff0c6-7a80-48f4-b5df-5946ba2e0ee2"><br/>

added file loading for my maps this added 3 - 6ms and was moved into the init for start time impact and not during game impact<br/>
<img width="1676" alt="profiling" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/313078a5-5889-49c8-9f82-d20bd1d051d5"><br/>

Using the CPU usage it is easier to tell where the clock cycles are taking the longest amount of time<br/>
in this case update takes the longest because it calls draw before completing which contains the delay but also contains all the rendering<br/>
<img width="1918" alt="Screenshot 2023-12-16 005642" src="https://github.com/Birmingham-City-Uni/milestones-ClaymoreKiwi/assets/109832174/04242d7c-9ed5-4a9c-bce0-d90976b2c9c0">

1. Method with its percentage of usage
2. Percentage of usage based on how long called methods/functions
3. Section of code uder analysis
4. Resources used by the CPU in the time frame

## References
Let's Make Games (2018). How To Make A Game #20 : 2D Player Follow Camera in C++ And SDL2 Tutorial. [online] www.youtube.com. Available at: https://www.youtube.com/watch?app=desktop&v=QeN1ygJD5y4 [Accessed 20 Nov. 2023].

le Duc, P. (2015). How to calculate delta time with SDL? [online] Game Development Stack Exchange. Available at: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl [Accessed 16 Dec. 2023].

Foo, L. (2014). Lazy Foo’ Productions. [online] lazyfoo.net. Available at: https://lazyfoo.net/SDL_tutorials/lesson17/index.php [Accessed 16 Dec. 2023].

CobaltHex (2012). How do I shoot bullets towards the mouse in a top down shooter? [online] Game Development Stack Exchange. Available at: https://gamedev.stackexchange.com/questions/24557/how-do-i-shoot-bullets-towards-the-mouse-in-a-top-down-shooter [Accessed 16 Dec. 2023].





