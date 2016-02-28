Guard = {
InformationComponent = {
name = "Guard",
position = {21,15},
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
aiSightLength = 3,
},
WaypointComponent = {
numberOfPoints = 4,
point2 = {21,8},
point3 = {3,8},
point4 = {3,23},
point1 = {21,23},
},
}
