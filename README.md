# C++ Animation Systems

Exploring core animation programming concepts in C++ by implementing basic systems commonly used in game engines.

## Systems Implemented

This repository contains implementations of:

- **State Machine**: Manages character animation states (Idle, Walk, Run, Jump) with conditional transitions
- **Blend Tree 1D**: Blends animations based on a parameter (e.g., movement speed)
- **Skeleton Hierarchy**: Hierarchical bone structure with transform propagation using Data-Oriented Design (SOA layout)
- **Pose Blending**: Blends multiple animation poses with weight normalization
- **Two-Bone IK Solver**: Inverse Kinematics solver using the law of cosines for analytical solutions

## Project Structure
```
Simple-Cpp-Animation-System/
├── Headers/
│   ├── StateMachine.h
│   ├── BlendTree1D.h
│   ├── Skeleton.h
│   ├── AnimationBlending.h
│   └── IKSolver.h
├── Sources/
│   ├── StateMachine.cpp
│   ├── BlendTree1D.cpp
│   ├── Skeleton.cpp
│   ├── AnimationBlending.cpp
│   └── IKSolver.cpp
├── main.cpp
└── README.md
```

## Technical Highlights

- **Data-Oriented Design**: Skeleton system uses Structure of Arrays (SOA) for cache-friendly bone transforms
- **Mathematical Foundations**: Custom Vector3, Quaternion, Matrix4x4, and Transform implementations

## Notes

This is an educational project focused on understanding animation system fundamentals from scratch. All math utilities and animation algorithms are implemented manually to deepen understanding of the underlying concepts.