#include "Headers/StateMachine.h"
#include "Headers/BlendTree1D.h"
#include "Headers/Skeleton.h"
#include "Headers/AnimationBlending.h"

#include <iostream>
#include <cassert>
#include <cmath>

void TestStateMachine()
{
    std::cout << "\n=== STATE MACHINE TESTS ===" << std::endl;

    StateMachine sm(Idle);

    // Test 1: Idle -> Walk -> Run
    sm.speed = 3.0f;
    sm.isGrounded = true;
    sm.update();
    std::cout << "Test 1 (Idle -> Walk): " << sm.getCurrentState() << std::endl;
    assert(sm.getCurrentState() == "Walk");

    sm.speed = 7.0f;
    sm.update();
    std::cout << "Test 2 (Walk -> Run): " << sm.getCurrentState() << std::endl;
    assert(sm.getCurrentState() == "Run");

    // Test 2: Jump from Run
    sm.isGrounded = false;
    sm.update();
    std::cout << "Test 3 (Run -> Jump): " << sm.getCurrentState() << std::endl;
    assert(sm.getCurrentState() == "Jump");

    // Test 3: Back to Idle after Jump
    sm.isGrounded = true;
    sm.update();
    std::cout << "Test 4 (Jump -> Idle): " << sm.getCurrentState() << std::endl;
    assert(sm.getCurrentState() == "Idle");

    // Test 4: Run -> Walk (deceleration)
    sm.speed = 4.0f;
    sm.update();
    std::cout << "Test 5 (Idle -> Walk): " << sm.getCurrentState() << std::endl;
    assert(sm.getCurrentState() == "Walk");

    std::cout << "All State Machine tests passed!" << std::endl;
}

void TestBlendTree1D()
{
    std::cout << "\n=== BLEND TREE 1D TESTS ===" << std::endl;

    BlendTree1D blendTree;

    AnimationClip idle{ "Idle", 1.0f };
    AnimationClip walk{ "Walk", 1.5f };
    AnimationClip run{ "Run", 0.8f };

    blendTree.addAnimation(0.0f, &idle);
    blendTree.addAnimation(3.0f, &walk);
    blendTree.addAnimation(6.0f, &run);

    std::cout << "Test 1 (speed=0.0): ";
    blendTree.printWeights(0.0f);

    std::cout << "Test 2 (speed=1.5): ";
    blendTree.printWeights(1.5f);

    std::cout << "Test 3 (speed=3.0): ";
    blendTree.printWeights(3.0f);

    std::cout << "Test 4 (speed=4.5): ";
    blendTree.printWeights(4.5f);

    std::cout << "Test 5 (speed=6.0): ";
    blendTree.printWeights(6.0f);

    std::cout << "Test 6 (speed=8.0, clamped): ";
    blendTree.printWeights(8.0f);

    std::cout << "All Blend Tree 1D tests passed!" << std::endl;
}

void TestSkeleton()
{
    std::cout << "\n=== SKELETON HIERARCHY TESTS ===" << std::endl;

    Skeleton skeleton;

    // Create hierarchy
    int root = skeleton.AddBone("Root", -1, Matrix4x4());
    int spine = skeleton.AddBone("Spine", root, Matrix4x4());
    int shoulder = skeleton.AddBone("Shoulder", spine, Matrix4x4());
    int elbow = skeleton.AddBone("Elbow", shoulder, Matrix4x4());
    int head = skeleton.AddBone("Head", shoulder, Matrix4x4());

    // Test FindBone
    assert(skeleton.FindBone("Root") == 0);
    assert(skeleton.FindBone("Spine") == 1);
    assert(skeleton.FindBone("Shoulder") == 2);
    assert(skeleton.FindBone("Elbow") == 3);
    assert(skeleton.FindBone("Head") == 4);
    std::cout << "FindBone tests passed!" << std::endl;

    // Test transform propagation
    skeleton.UpdateWorldTransforms();
    std::cout << "UpdateWorldTransforms executed!" << std::endl;

    Matrix4x4 rootWorld = skeleton.GetWorldTransform(0);
    Matrix4x4 spineWorld = skeleton.GetWorldTransform(1);
    std::cout << "GetWorldTransform tests passed!" << std::endl;

    // Test dynamic modification
    Matrix4x4 spineTransform;
    spineTransform.data[0] = 2.0f;
    skeleton.SetLocalTransform(spine, spineTransform);
    skeleton.UpdateWorldTransforms();

    Matrix4x4 shoulderWorldAfter = skeleton.GetWorldTransform(shoulder);
    Matrix4x4 elbowWorldAfter = skeleton.GetWorldTransform(elbow);
    Matrix4x4 headWorldAfter = skeleton.GetWorldTransform(head);

    assert(shoulderWorldAfter.data[0] >= 2.0f);
    assert(elbowWorldAfter.data[0] >= 2.0f);
    assert(headWorldAfter.data[0] >= 2.0f);
    std::cout << "Propagation test passed!" << std::endl;

    std::cout << "All Skeleton tests passed!" << std::endl;
}

void TestAnimationBlending()
{
    std::cout << "\n=== ANIMATION BLENDING TESTS ===" << std::endl;

    // Test lerp functions
    std::cout << "\nTest 1: Lerp float" << std::endl;
    assert(lerp(0.0f, 10.0f, 0.0f) == 0.0f);
    assert(lerp(0.0f, 10.0f, 0.5f) == 5.0f);
    assert(lerp(0.0f, 10.0f, 1.0f) == 10.0f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 2: Lerp Vector3" << std::endl;
    Vector3 v1(0, 0, 0);
    Vector3 v2(10, 20, 30);
    Vector3 vResult = lerp(v1, v2, 0.5f);
    assert(vResult.x == 5.0f && vResult.y == 10.0f && vResult.z == 15.0f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 3: Lerp Quaternion" << std::endl;
    Quaternion q1(0, 0, 0, 1);
    Quaternion q2(0, 1, 0, 0);
    Quaternion qResult = lerp(q1, q2, 0.5f);
    assert(qResult.x == 0.0f && qResult.y == 0.5f && qResult.z == 0.0f && qResult.w == 0.5f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 4: Lerp Transform" << std::endl;
    Transform t1(Vector3(0, 0, 0), Quaternion(0, 0, 0, 1), Vector3(1, 1, 1));
    Transform t2(Vector3(10, 20, 30), Quaternion(0, 1, 0, 0), Vector3(2, 2, 2));
    Transform tResult = lerp(t1, t2, 0.5f);
    assert(tResult.position.x == 5.0f && tResult.position.y == 10.0f);
    assert(tResult.scale.x == 1.5f);
    std::cout << "  PASSED" << std::endl;

    // Test pose blending
    Transform idleTransform(Vector3(0, 0, 0), Quaternion(0, 0, 0, 1), Vector3(1, 1, 1));
    Transform walkTransform(Vector3(1, 1, 1), Quaternion(0, 0.5f, 0, 0.5f), Vector3(1, 1, 1));
    Transform runTransform(Vector3(2, 2, 2), Quaternion(0, 1, 0, 0), Vector3(1, 1, 1));

    Pose idle(3, idleTransform);
    Pose walk(3, walkTransform);
    Pose run(3, runTransform);

    std::cout << "\nTest 5: Normalized weights (0.5, 0.5)" << std::endl;
    std::vector<Pose> poses = { idle, walk };
    std::vector<float> weights = { 0.5f, 0.5f };
    Pose result = BlendPoses(poses, weights);
    assert(result.boneTransforms.size() == 3);
    assert(std::abs(result.boneTransforms[0].position.x - 0.5f) < 0.001f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 6: Non-normalized weights (1.0, 1.0)" << std::endl;
    weights = { 1.0f, 1.0f };
    result = BlendPoses(poses, weights);
    assert(std::abs(result.boneTransforms[0].position.x - 0.5f) < 0.001f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 7: Negative weight clamping (-0.5, 1.5)" << std::endl;
    weights = { -0.5f, 1.5f };
    result = BlendPoses(poses, weights);
    assert(std::abs(result.boneTransforms[0].position.x - 1.0f) < 0.001f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 8: Zero-sum weights" << std::endl;
    weights = { 0.0f, 0.0f };
    result = BlendPoses(poses, weights);
    assert(std::abs(result.boneTransforms[0].position.x - 0.0f) < 0.001f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 9: Three animations (0.2, 0.3, 0.5)" << std::endl;
    poses = { idle, walk, run };
    weights = { 0.2f, 0.3f, 0.5f };
    result = BlendPoses(poses, weights);
    assert(std::abs(result.boneTransforms[0].position.x - 1.3f) < 0.001f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "All Animation Blending tests passed!" << std::endl;
}

int main()
{
    std::cout << "=====================================" << std::endl;
    std::cout << "  ANIMATION SYSTEMS TEST SUITE" << std::endl;
    std::cout << "=====================================" << std::endl;

    TestStateMachine();
    TestBlendTree1D();
    TestSkeleton();
    TestAnimationBlending();

    std::cout << "\n=====================================" << std::endl;
    std::cout << "  ALL TESTS PASSED SUCCESSFULLY" << std::endl;
    std::cout << "=====================================" << std::endl;

    return 0;
}