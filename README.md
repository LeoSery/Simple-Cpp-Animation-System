# C++ Animation Systems

Exploring core animation programming concepts in C++ by implementing basic systems commonly used in game engines.

## Systems Implemented

This repository contains simple implementations of:

- **State Machine**: Manages character animation states (Idle, Walk, Run, Jump) with conditional transitions
- **Blend Tree 1D**: Blends animations based on a parameter (e.g., movement speed)
- **Skeleton Hierarchy**: Hierarchical bone structure with transform propagation
- **Pose Blending**: Blends multiple animation poses with weight normalization

## Project Structure

```
cpp-animation-systems/
├── src/
│   ├── main.cpp                    # Test runner
│   ├── StateMachine.h/.cpp         # State machine implementation
│   ├── BlendTree1D.h/.cpp          # Blend tree implementation
│   ├── Skeleton.h/.cpp             # Skeleton hierarchy
│   └── AnimationBlending.h/.cpp    # Pose blending system
└── README.md
```

## Notes

This is an educational project focused on understanding animation system fundamentals. Implementations are simplified for learning purposes.
Math utilities (Vector3, Quaternion, Transform) are provided helpers to focus on core animation concepts.