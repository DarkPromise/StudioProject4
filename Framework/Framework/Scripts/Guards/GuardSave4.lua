Guard = {
InformationComponent = {
name = "Guard",
position = {6,13},
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
aiState = "Idle",
aiType = "Guard",
aiDifficulty = "Normal",
aiSightLength = 2,
},
WaypointComponent = {
numberOfPoints = 2,
point2 = {6,13},
point1 = {6,13},
},
}
