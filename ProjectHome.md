SimpleFPS is a benchmark for evaluating the performance of planners in a planning domain that resembles a first-person shooter (FPS) game-world and planning problems related to tactical decision making for non-player characters in the game.

The SimpleFPS planning domain (sFPSgen.txt) captures some of the basic functionality of non-player characters (NPCs) that live and act in a FPS video game. Formally, the SimpleFPS planning domain is expressed in the STRIPS subset of the Planning Domain Definition Language (PDDL). PDDL is commonly used by academic planners, such as those participating in the International Planning Competition held by the ICAPS conference.

In the SimpleFPS planning domain the game-world is expressed using areas and points of interest. Areas capture large spaces where points of interest for the NPC may be found, such as weapons, ammo, med-kits, as well as doors or other way-points that connect areas.

sFPSgen is a tool for creating SimpleFPS planning problems expressed in PDDL for the SimpleFPS planning domain. sFPS-a5-c0.7, sFPS-a7-c0.7, sFPS-a10-c0.7, sFPS-a20-c0.7 are four collections of planning problems with 5, 7, 10, 20 areas respectively. The folder "Experiments" holds the logs and statistics from solving these problem-sets with the planner fastdownward.