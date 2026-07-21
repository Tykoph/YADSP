<p align="center">
    <a href="#">
        <img src="/Resources/Icon128.png">
    </a>
</p>

# Yet Another Dialogue System Plugin (YADSP)

This is a plugin for Unreal Engine 5 that aims to be a modular dialogue system easy to use for game designers and programmers alike.

![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)

## Features

- **Node-Based Editor:** A fully integrated visual graph editor allowing designers to create and manage dialogue trees without writing code.
- **World Integration:** Use the `ADialogueActor` to easily place dialogue triggers in your levels.
- **Dynamic Branching:** Create non-linear conversations using conditional branches.
- **Data-Driven Localization:** Deep integration with the `GSheetLocSystemPlugin` to fetch localized dialogue text and speaker names directly from Google Sheets DataTables.
- **Action Triggers:** Seamlessly integrated with `GameActionPlugin` to trigger gameplay events, logic, or variable changes mid-conversation.

## Getting Started

1. Setup RichText inside the YADSP project settings
2. Setup your Speaker and Dialogue DataTables via GSheetLocSystem.
3. Create a new `DialogueSystem` asset in the Content Browser, assign your Speaker and Dialogue DataTables and build your dilogue graph.
4. Add `DialogueSubsystem` somewhere in your game and assign the a Dialogue Widget.
5. Place a `DialogueActor` or add a `DialoguePlayer` to any actor in your scene, assign your `DialogueSystem` asset, and call `PlayDialogue()` via Blueprint or C++.

## Dependencies

YADSP relies on the following plugins to function:
- `GSheetLocSystemPlugin`
- `GameActionPlugin`
