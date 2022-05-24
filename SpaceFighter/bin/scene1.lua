STAR_SAIL = 1

EXPLSION_EFFECT_1 = 2
HERO_PLANE	= 		3
EXPLSION_EFFECT_2 = 4
EXPLSION_EFFECT_3 = 5
MISAILE_1 = 6
MISAILE_2 = 7
PLANE_1	  = 8
PLANE_2 = 9
PLANE_3 = 10
PLANE_4 = 11
ITEM_BOX = 12

------------------------------------------------------------------------------
OBJECT_PLAYER  = 0
OBJECT_ENEMY_GENERAL = 1
OBJECT_BOMB =  2
OBJECT_BOSS = 3
OBJECT_TRIGGER = 4

GameObjectDesc = {}
GameObjectDesc.ObjectType = OBJECT_ENEMY_GENERAL
GameObjectDesc.ResID = STAR_SAIL
GameObjectDesc.VelX = 0
GameObjectDesc.VelY = 0
GameObjectDesc.VelZ = -18

GameObjectDesc.PosY = 0
GameObjectDesc.PosZ = 120

GameObjectDesc.PosX = 40
GameObjectDesc.TimeLine = 1000
GameObjectDesc.AIPattern = 'AIPattern1'
AddPlanePool(GameObjectDesc)

-----------------------------------------------------------------------------

GameObjectDesc.PosX = 0
GameObjectDesc.TimeLine = 1000
AddPlanePool(GameObjectDesc)

GameObjectDesc.PosX = -40
GameObjectDesc.TimeLine = 1500
GameObjectDesc.AIPattern = 'AIStraight'
AddPlanePool(GameObjectDesc)

--GameObjectDesc.PosX = -40
--GameObjectDesc.TimeLine = 5500
--AddPlanePool(GameObjectDesc)

--GameObjectDesc.PosX = 0
--GameObjectDesc.TimeLine = 5500
--GameObjectDesc.AIPattern = 'AIStraight'
--AddPlanePool(GameObjectDesc)

--GameObjectDesc.PosX = 40
--GameObjectDesc.TimeLine = 5500
--GameObjectDesc.AIPattern = 'AIStraight'
--AddPlanePool(GameObjectDesc)

--GameObjectDesc.PosX = 90
--GameObjectDesc.TimeLine = 12000
--AddPlanePool(GameObjectDesc)

--GameObjectDesc.PosX = -10
--GameObjectDesc.TimeLine = 12000
--AddPlanePool(GameObjectDesc)

GameObjectDesc.ObjectType = OBJECT_BOMB
GameObjectDesc.ResID = EXPLSION_EFFECT_1
GameObjectDesc.AIPattern = 'NULL'
GameObjectDesc.PosX = 0
GameObjectDesc.PosY = 0
GameObjectDesc.PosZ = 0
GameObjectDesc.TimeLine = 2000
GameObjectDesc.VelX = 0
GameObjectDesc.VelY = 0
GameObjectDesc.VelZ = 0
AddPlanePool(GameObjectDesc)

GameObjectDesc.ObjectType = OBJECT_TRIGGER
GameObjectDesc.ResID = STAR_SAIL
GameObjectDesc.AIPattern = 'AIStraight'
GameObjectDesc.PosX = 80
GameObjectDesc.PosY = 0
GameObjectDesc.PosZ = 120
GameObjectDesc.TimeLine = 2100
GameObjectDesc.VelX = 0
GameObjectDesc.VelY = 0
GameObjectDesc.VelZ = -18
AddPlanePool(GameObjectDesc)