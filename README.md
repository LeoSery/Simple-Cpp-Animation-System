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

## Ressources

### Motion Matching
- [GDC 2016 - Simon Clavet "Motion Matching and The Road to Next-Gen Animation"](https://www.gdcvault.com/play/1023280/Motion-Matching-and-The-Road)
- [Game Anim - Motion Matching & For Honor](https://www.gameanim.com/2016/05/03/motion-matching-ubisofts-honor/)
- [Ubisoft Montreal Blog - Motion Matching](https://montreal.ubisoft.com/en/tag/motionmatching/)

### Learned Motion Matching
- [Ubisoft La Forge - Introducing Learned Motion Matching](https://www.ubisoft.com/en-us/studio/laforge/news/6xXL85Q3bF2vEj76xmnmIu/introducing-learned-motion-matching)
- [Ubisoft La Forge - Learned Motion Matching (article complet)](https://www.ubisoft.com/en-us/studio/laforge/news/3NWqwcMU9lMfumgfPAEfka/learned-motion-matching)

### Animation Compression
- [Nicholas Frechette Blog - Animation Compression (ACL author)](https://nfrechette.github.io/2016/10/21/anim_compression_toc/)

### Skinning
- [Gabor Makes Games - Skinning](https://gabormakesgames.com/blog_animation_skinspace.html)
- [WebGL Fundamentals - Skinning](https://webglfundamentals.org/webgl/lessons/webgl-skinning.html)

### State of the Art (2024-2025)
- [GDC 2024 - Tencent MotorNerve Animation System](https://gdcvault.com/play/1034825/MotorNerve-A-Character-Animation-System)
- [Ubisoft LaFAN1 Dataset (GitHub)](https://github.com/ubisoft/ubisoft-laforge-animation-dataset)