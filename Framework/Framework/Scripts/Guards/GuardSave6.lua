Guard = {
InformationComponent = {
name = "Guard",
position = {14,2},
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
numberOfPoints = 10,
point2 = {14,2},
point3 = {17,2},
point4 = {14,2},
point5 = {14,4},
point6 = {17,4},
point7 = {17,6},
point8 = {10,6},
point9 = {17,6},
point10 = {17,4},
point1 = {14,4},
},
}
