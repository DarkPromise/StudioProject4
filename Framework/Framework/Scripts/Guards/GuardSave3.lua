Guard = {
InformationComponent = {
name = "Guard",
position = {10,22},
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
aiSightLength = 0,
},
WaypointComponent = {
numberOfPoints = 4,
point2 = {10,16},
point3 = {14,16},
point4 = {14,22},
point1 = {10,22},
},
}
