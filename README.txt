Controlling the Camera: Right click and hold in the viewport to take control of the camera. The following controls work only when right clicking and holding.
	- [ Mouse X & Y ] 	Look Around the World
	- [ WASD ] 	Move Around the World
	- [ Q ] 		Move Directly Up
	- [ E ] 		Move Directly Down
	- [ N ] 		Increase FOV
	- [ M ] 		Decrease FOV
	- [ B ] 		Increase Camera Movement Speed
	- [ V ] 		Decrease Camera Movement Speed
	- [ P ] 		Increase Near Clip Distance
	- [ O ] 		Decrease Near Clip Distance
	- [ I ] 		Increase Far Clip Distance
	- [ U ] 		Decrease Far Clip Distance
	- [ L-Shift ]		Rotate directional light.

The following controls can only be done when not holding down the RMB.
	- [ Tab ] 		Make the camera look at the cube that is rising up.
	- [ L-Shift ]		Rotate directional light.

The following controls are for when you are using 'Tab' to look at an object.
	- [ W ] 		Move closer to object.
	- [ S ] 		Move farther from object.

Notes:
	- The spotlight is the red light, and it moves and rotates on a sin wave to move in circles.
	- The emissive material is on the cube that is lighting up. It is the same cube as the rest, just this one has an emissive brightness of { 3, 3, 3, 3 }.
	- There is also a Bamboo with Emissive material on it. It doesn't load the mesh right yet, but it does use the alpha value of the texture to calculate light in the pixel shader.
	- The planet is also slightly emissive, and it swirls from a vertex shader, so I am calling it lava!

	- The point light slowly rotates about and falls down from the scene, so the green light is the point light and will eventually disappear to show that it is dynamic.
	- The cube will light up for the directional light, and get dim and dark for the ambient light.