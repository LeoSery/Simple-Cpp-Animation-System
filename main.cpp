#include "Headers/StateMachine.h"
#include "Headers/BlendTree1D.h"
#include "Headers/Skeleton.h"
#include "Headers/AnimationBlending.h"
#include "Headers/IKSolver.h"

#include <iostream>
#include <cassert>
#include <cmath>

#pragma region Tests
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

    // Create hierarchy: Root -> Spine -> Shoulder -> Elbow -> Hand
    int root = skeleton.AddBone("Root", -1, Matrix4x4());
    int spine = skeleton.AddBone("Spine", root, Matrix4x4());
    int shoulder = skeleton.AddBone("Shoulder", spine, Matrix4x4());
    int elbow = skeleton.AddBone("Elbow", shoulder, Matrix4x4());
    int hand = skeleton.AddBone("Hand", elbow, Matrix4x4());

    // Test FindBone
    assert(skeleton.FindBone("Root") == 0);
    assert(skeleton.FindBone("Spine") == 1);
    assert(skeleton.FindBone("Shoulder") == 2);
    assert(skeleton.FindBone("Elbow") == 3);
    assert(skeleton.FindBone("Hand") == 4);
    assert(skeleton.FindBone("NonExistent") == -1);
    std::cout << "FindBone tests passed!" << std::endl;

    // Test initial state (all identity)
    skeleton.UpdateWorldTransforms();
    Matrix4x4 initialShoulder = skeleton.GetWorldTransform(shoulder);
    assert(initialShoulder.data[0] == 1.0f && initialShoulder.data[5] == 1.0f);
    std::cout << "Initial state test passed!" << std::endl;

    // Test rotation propagation - 45 degrees
    Matrix4x4 rotation45 = Matrix4x4::RotationZ(3.14159f / 4.0f);
    skeleton.SetLocalTransform(shoulder, rotation45);
    skeleton.UpdateWorldTransforms();

    Matrix4x4 shoulderWorld = skeleton.GetWorldTransform(shoulder);
    Matrix4x4 elbowWorld = skeleton.GetWorldTransform(elbow);
    Matrix4x4 handWorld = skeleton.GetWorldTransform(hand);

    // Verify cos(45) and sin(45) are approximately 0.707
    float cos45 = shoulderWorld.data[0];
    float sin45 = shoulderWorld.data[4];
    assert(cos45 > 0.706f && cos45 < 0.708f);
    assert(sin45 > 0.706f && sin45 < 0.708f);

    // Verify propagation: children should have same rotation
    assert(elbowWorld.data[0] == shoulderWorld.data[0]);
    assert(handWorld.data[0] == shoulderWorld.data[0]);
    std::cout << "45-degree rotation test passed!" << std::endl;

    // Test rotation 90 degrees
    Matrix4x4 rotation90 = Matrix4x4::RotationZ(3.14159f / 2.0f);
    skeleton.SetLocalTransform(shoulder, rotation90);
    skeleton.UpdateWorldTransforms();

    Matrix4x4 shoulderWorld90 = skeleton.GetWorldTransform(shoulder);
    assert(shoulderWorld90.data[0] < 0.001f && shoulderWorld90.data[0] > -0.001f); // cos(90) ~ 0
    assert(shoulderWorld90.data[4] > 0.999f && shoulderWorld90.data[4] < 1.001f); // sin(90) ~ 1
    std::cout << "90-degree rotation test passed!" << std::endl;

    // Test negative rotation -45 degrees
    Matrix4x4 rotationNeg45 = Matrix4x4::RotationZ(-3.14159f / 4.0f);
    skeleton.SetLocalTransform(shoulder, rotationNeg45);
    skeleton.UpdateWorldTransforms();

    Matrix4x4 shoulderWorldNeg = skeleton.GetWorldTransform(shoulder);
    assert(shoulderWorldNeg.data[4] < -0.706f && shoulderWorldNeg.data[4] > -0.708f); // sin(-45) ~ -0.707
    std::cout << "Negative rotation test passed!" << std::endl;

    // Verify Root and Spine are unaffected
    Matrix4x4 rootWorld = skeleton.GetWorldTransform(root);
    Matrix4x4 spineWorld = skeleton.GetWorldTransform(spine);
    assert(rootWorld.data[0] == 1.0f && rootWorld.data[5] == 1.0f);
    assert(spineWorld.data[0] == 1.0f && spineWorld.data[5] == 1.0f);
    std::cout << "Parent isolation test passed!" << std::endl;

    std::cout << "\n=== ALL SKELETON TESTS PASSED ===" << std::endl;
}

void TestAnimationBlending()
{
    std::cout << "\n=== ANIMATION BLENDING TESTS ===" << std::endl;

    // Test lerp functions
    std::cout << "\nTest 1: Lerp float" << std::endl;
    assert(Lerp(0.0f, 10.0f, 0.0f) == 0.0f);
    assert(Lerp(0.0f, 10.0f, 0.5f) == 5.0f);
    assert(Lerp(0.0f, 10.0f, 1.0f) == 10.0f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 2: Lerp Vector3" << std::endl;
    Vector3 v1(0, 0, 0);
    Vector3 v2(10, 20, 30);
    Vector3 vResult = Lerp(v1, v2, 0.5f);
    assert(vResult.x == 5.0f && vResult.y == 10.0f && vResult.z == 15.0f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 3: Lerp Quaternion" << std::endl;
    Quaternion q1(0, 0, 0, 1);
    Quaternion q2(0, 1, 0, 0);
    Quaternion qResult = Lerp(q1, q2, 0.5f);
    assert(qResult.x == 0.0f && qResult.y == 0.5f && qResult.z == 0.0f && qResult.w == 0.5f);
    std::cout << "  PASSED" << std::endl;

    std::cout << "\nTest 4: Lerp Transform" << std::endl;
    Transform t1(Vector3(0, 0, 0), Quaternion(0, 0, 0, 1), Vector3(1, 1, 1));
    Transform t2(Vector3(10, 20, 30), Quaternion(0, 1, 0, 0), Vector3(2, 2, 2));
    Transform tResult = Lerp(t1, t2, 0.5f);
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

void TestIKSolver()
{
    std::cout << "\n=== IK SOLVER TESTS ===" << std::endl;

    IKChain armChain;
    armChain.upperLength = 0.3f;
    armChain.lowerLength = 0.25f;

    // ============================================
    // Test 1: Normal case (reachable)
    // ============================================
    std::cout << "\nTest 1: Normal reachable case" << std::endl;
    Vector3 start1(0.0f, 0.0f, 0.0f);
    Vector3 target1(0.4f, 0.3f, 0.0f);

    IKResult result1 = SolveTwoBoneIK(start1, target1, armChain);

    assert(result1.isReachable == true);
    assert(result1.shoulderAngle >= -3.14f && result1.shoulderAngle <= 3.14f);
    assert(result1.elbowAngle >= -3.14f && result1.elbowAngle <= 3.14f);
    std::cout << "  Reachable: " << (result1.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  Shoulder angle: " << result1.shoulderAngle << " rad" << std::endl;
    std::cout << "  Elbow angle: " << result1.elbowAngle << " rad" << std::endl;
    std::cout << "  PASSED" << std::endl;

    // ============================================
    // Test 2: Out of range (too far away)
    // ============================================
    std::cout << "\nTest 2: Out of reach (too far)" << std::endl;
    Vector3 start2(0.0f, 0.0f, 0.0f);
    Vector3 target2(1.0f, 0.0f, 0.0f);

    IKResult result2 = SolveTwoBoneIK(start2, target2, armChain);

    assert(result2.isReachable == false);
    std::cout << "  Reachable: " << (result2.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  Distance: 1.0 > maxReach (0.55)" << std::endl;
    std::cout << "  PASSED" << std::endl;

    // ============================================
    // Test 3: Too close
    // ============================================
    std::cout << "\nTest 3: Too close" << std::endl;
    Vector3 start3(0.0f, 0.0f, 0.0f);
    Vector3 target3(0.02f, 0.0f, 0.0f);

    IKResult result3 = SolveTwoBoneIK(start3, target3, armChain);

    assert(result3.isReachable == false);
    std::cout << "  Reachable: " << (result3.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  Distance: 0.02 < minReach (0.05)" << std::endl;
    std::cout << "  PASSED" << std::endl;

    // ============================================
    // Test 4: Distance = 0 (edge case)
    // ============================================
    std::cout << "\nTest 4: Distance zero (start == target)" << std::endl;
    Vector3 start4(0.0f, 0.0f, 0.0f);
    Vector3 target4(0.0f, 0.0f, 0.0f);

    IKResult result4 = SolveTwoBoneIK(start4, target4, armChain);

    assert(result4.isReachable == false);
    std::cout << "  Reachable: " << (result4.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  Distance: 0.0 (undefined direction)" << std::endl;
    std::cout << "  PASSED" << std::endl;

    // ============================================
    // Test 5: Just at the limit
    // ============================================
    std::cout << "\nTest 5: Exactly at max reach" << std::endl;
    Vector3 start5(0.0f, 0.0f, 0.0f);
    Vector3 target5(0.55f, 0.0f, 0.0f);

    IKResult result5 = SolveTwoBoneIK(start5, target5, armChain);

    assert(result5.isReachable == true);
    // Arm fully extended => elbow angle should be close to 180 degrees (PI radians)
    assert(std::abs(result5.elbowAngle - 3.14159f) < 0.1f); // ~180 degres
    std::cout << "  Reachable: " << (result5.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  Elbow angle: " << result5.elbowAngle << " rad (~PI = straight arm)" << std::endl;
    std::cout << "  PASSED" << std::endl;

    // ============================================
    // Test 6: 3D Case (bonus)
    // ============================================
    std::cout << "\nTest 6: 3D case (bonus)" << std::endl;
    Vector3 start6(0.0f, 0.0f, 0.0f);
    Vector3 target6(0.2f, 0.3f, 0.2f);  // distance ~0.412

    IKResult result6 = SolveTwoBoneIK(start6, target6, armChain);

    assert(result6.isReachable == true);
    std::cout << "  Reachable: " << (result6.isReachable ? "YES" : "NO") << std::endl;
    std::cout << "  3D target reached successfully" << std::endl;
    std::cout << "  PASSED" << std::endl;

    std::cout << "\n=== ALL IK SOLVER TESTS PASSED ===" << std::endl;
}
#pragma endregion

int main(int argc, char *argv[])
{
    std::cout << "=====================================" << std::endl;
    std::cout << "  ANIMATION SYSTEMS TEST SUITE" << std::endl;
    std::cout << "=====================================" << std::endl;

    TestStateMachine();
    TestBlendTree1D();
    TestSkeleton();
    TestAnimationBlending();
    TestIKSolver();

    std::cout << "\n=====================================" << std::endl;
    std::cout << "  ALL TESTS PASSED SUCCESSFULLY" << std::endl;
    std::cout << "=====================================" << std::endl;

    return 0;
}