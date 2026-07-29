---
name: week1
description: You need to move the engine-level code you have already written into the MegaManWinAPI project so that the main game loop runs.
---

# Week 1 Goal: Frame skeleton grafting

The main goal of this week’s session is for users to directly copy the engine-level code from existing `Game1945` or `BounceBall` projects into `MegaManWin`Create an API project and make sure the basic game loop works properly.

## Agent Guidelines

Once this technology is activated, the agent must carry out tasks and coach the user according to the following steps.

### Step 1: File mobility notice (user action required)
We guide users to copy the following key file list into `MegaManWinAPI/MegaManWinCreate an API folder and include it in your project using the 'Add Existing Item' feature in Visual Studio Solution Explorer. 
(※ The agent does not directly copy the file using the script, and waits until the user responds that they have completed copying and adding after receiving the notification.)

**List of core files to port:**
- **Core and Loop: game.h / game.cpp, scene.h / scene.cpp
- **Entity System:** actor.h / `Actor.cpp`, component.h / `Component.cpp`
- **Manager class (Singleton):**
  - `time_manager.h` / `TimeManager.cpp`
  - `InputManager.h` / `InputManager.cpp`
  - `resource-manager.h` / `ResourceManager.cpp`
  - `CollisionManager.h` / `CollisionManager.cpp`
  - `SoundManager.h` / `SoundManager.cpp` (for sound playback)
- **Rendering System:** `SpriteRenderer.h` / `SpriteRenderer.cpp`, `ImageRenderer.h` / `ImageRenderer.cpp`
- **Other utilities:** Singleton.Usage.h` / `Util.cpp`

### Steps 1-2: Implementation and upgrade (new!) of the additional system
Beyond simple porting, it guides you on how to configure the essential features needed for MegaMan emulation together in the first week.
- **Creating an AnimatorComponent:** We propose a class design for an `AnimatorComponent` that can professionally handle frame animations according to various character states such as being a child, running, or jumping, and we encourage you to design and implement it.

### Step 2: Code Compatibility Review and Modification Suggestions
Once the user completes adding the file, the agent should review the code and propose ** changes to ensure the ported file works in the new project environment.
- **Key Review Items:**
  - Make sure the relevant path settings include `pch.h` (the precompiled header)
  - Remove the namespace and existing project dependencies (if necessary)
  - Modified the include path to fit the new MegaMan project structure
  - Verify the main game loop structure of the `Game` class

### Step 3: Confirm Completion
Once the code modification proposal and its implementation are completed, the agent verifies the follo