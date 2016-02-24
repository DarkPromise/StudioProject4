Player = {
InformationComponent = {
    name = "Josh",
    position = {0,0},
    direction = {1,0,0},
    type = "Player",
  },
GraphicsComponent = {
    meshType = "Quad",
    meshName = "Player",
    meshImage = "Images//player.tga",
    meshSize = 32,
    meshAlpha = 0.7,
    meshColor = {1,0,0},
  },
CameraComponent = {
    cameraOffset = {0,0,300},
    cameraMode = 2,
  },
ControllerComponent = {
  },
CollisionComponent = {
    min = {-5,-5,-5},
    max = {5,5,5},
  },
HealthComponent = {
    minHp = "100",
    maxHp = "100",
  },
 GameplayComponent = {
    currLevel = 1,
    hasKey = "false",
  },
}
