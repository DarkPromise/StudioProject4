Guard = {
InformationComponent = {
name = "Guard",
position = {8,22},
direction = {-1,0,0},
rotation = {-90,0,0},
type = "NPC",
},
GraphicsComponent = {
meshType = "Quad",
meshName = "Guard Mesh",
meshImage = "Images//guard.tga",
meshSize = 32,
meshColor = {1, 1, 1},
meshAlpha = 1,
},
AIComponent = {
aiState = "Patrol",
aiType = "Guard",
aiDifficulty = "Normal",
aiSightLength = 5,
},
WaypointComponent = {
numberOfPoints = 6,
point2 = {8,22},
point3 = {6,22},
point4 = {8,22},
point5 = {8,25},
point6 = {6,25},
point1 = {8,25},
},
}
