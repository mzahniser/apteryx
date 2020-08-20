# Apteryx

Apteryx is an action / puzzle game which requires both quick reflexes and strategic thinking. The goal of the game is to make your way through 11 levels. You will collect a variety of weapons, but in many situations the best strategy will be simply to trick enemies into crashing into each other.

Your path will often be blocked by force fields, which can only be destroyed by manipulating an enemy drone into crashing into the switch that controls the field. In the early game, this can be done simply by tricking the drone into crashing into the switch while chasing you. Later on, you will collect weapons such as the tractor beam and repulsor that can be used to move enemies around.


## Controls

For now, the controls are hard-coded. They include:

**Ctrl-N:** New game. Returns you to the start of the map. If you want to try out some weapons before beginning the actual game, press 'Z' at the start to travel up to the tutorial level. The tutorial level has no exit, so you must start a new game when you are done exploring it.

**Ctrl-S:** Save current game state (freeze). Currently there is only one freeze slot, so be careful not to freeze the game unless you are sure it is not in an unwinnable state.

**Ctrl-O:** Open the saved game (defrost).

**Ctrl-P:** Pause the game, bringing up the main menu. Or if paused already, continue.

**Ctrl-K:** Bring up the help screen, listing the keyboard controls.

**Ctrl-Q:** Quit Apteryx.

**Arrow keys:** Move your hovercraft. You have four engines facing in the four cardinal directions, so you can accelerate equally quickly in any direction.

**Z:** Tunnel up. If your hovercraft is over a square with an X on it, you will travel up to the previous level. Your hovercraft must be entirely within the square and must be stationary or moving slowly.

**X:** Tunnel down:** If your hovercraft is over a hole in the ground, you will travel down into the next level. As with tunneling up, you must be centered on the hole and not moving too quickly.

**Left Shift:** Fire your laser. Lasers (and all other weapons) travel in the direction that your hovercraft is moving in. Lasers use up energy, which must be replenished by picking up yellow canisters.

**S:** Select special weapon. If you have picked up any other weapons, this key cycles through them. The different weapons are described below.

**A:** Fire special weapon. Some weapons have a limited number of shots; others draw energy from your ship. Many of the weapons that draw energy have a continuous effect; for example, the cloaking device makes your hovercraft invisible to enemies for as long as you hold down the special trigger.

**Tab:** If you control any of the enemies in the level (or if you have launched a spy bot), press Tab to cycle your view and control between them.

Other keys available mostly for debugging:

**Ctrl-C:** Activate "cheater mouse" control. Your velocity will be controlled by the mouse position (unlimited acceleration), and you can click the mouse to fire.

**V:** Toggle the visibility layer (shadows). If you are on a slow computer, turning off shadows will make the game run much smoother.



## Enemies

**Red drone:** Has no weapons, aside from the ability to crash into you, and is not very smart at chasing you. If you move away from a red drone at a constant speed, it will follow you without ever catching up.

**Yellow drone:** Same as the red drone, but with a smarter AI. It will extrapolate your future position based on your current velocity, so the only way to dodge one of these is to constantly change directions.

**Green drone:** Like a red drone, but with lasers.

**Blue drone:** Like a yellow drone, but with lasers.

**Grey drone:** Is not destroyed by anything but a collision with a switch. Can be put to sleep with an EMP projectile or an EMP Burst, and can be moved or pushed away with the tractor beam or repulsor. As with all enemies, can be captured with the virus.



## Weapons

**Lasers:** Your primary weapon. The more lasers you pick up, the faster they fire. Most drones can be destroyed by three hits from a laser. The laser impact also pushes drones backwards, which is useful in some situations.

**Proximity Mine:** Destroys any drone that crashes into it. If you don't have mines, try pushing the dead remains of a drone into a room and hiding behind it, for much the same effect.

**Electro-Magnetic Pulse:** Any enemy hit by this projectile will be put to sleep for several seconds.

**Virus:** When an enemy is hit by the virus, you take over control of it. Use the Tab key to switch control to that enemy. You can now fly it around, doing whatever you want with it. This is very helpful for manipulating a drone into crashing into a hard-to-reach switch.

**Fusion Cannon:** When a fusion projectile hits a live drone, it destroys that drone and splits into three more fusion projectiles. This is very useful against large groups.

**Afterburner:** While the trigger is held down, increases your acceleration.

**Repulsor:** Pushes all drones within your field of vision directly away from you.

**Tractor beam:** Causes all drones within your field of vision to match velocity with you. This allows you to maneuver them in any direction you want.

**Cloaking device:** When activated, drones will not be able to detect you and will instead move towards wherever they last remember seeing you.

**EMP Burst:** All visible enemies will be put to sleep, and will sleep for a longer time than enemies affected by the ordinary EMP weapon. This uses a lot of energy.

**Emergency Hexagon:** A temporary protective shield.



## Other items

**Radar:** Activates the map. Blinking red dots are enemies; yellow dots are upgrades. Other colored squares are either force fields or the corresponding switches. Your hovercraft is shown as a white dot.

**Blue canisters:** Refill your shields.

**Yellow canisters:** Recharge your energy.

**Shield generator:** Causes your shields to slowly recharge. Collecting multiple generators increases the recharge rate.

**Energy generator:** Same as the shield generator, but for energy.
