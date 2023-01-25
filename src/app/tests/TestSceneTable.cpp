/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <app/clusters/scenes/SceneTableImpl.h>
#include <lib/support/TestPersistentStorageDelegate.h>
#include <lib/support/UnitTestRegistration.h>
#include <nlunit-test.h>

using FabricIndex     = chip::FabricIndex;
using SceneTableEntry = chip::scenes::DefaultSceneTableImpl::SceneTableEntry;
using SceneTableImpl  = chip::scenes::DefaultSceneTableImpl;
using SceneStorageId  = chip::scenes::DefaultSceneTableImpl::SceneStorageId;
using SceneData       = chip::scenes::DefaultSceneTableImpl::SceneData;

namespace {

static chip::TestPersistentStorageDelegate testStorage;
static SceneTableImpl sSceneTable;

// Test fabrics, adding more requires to modify the "ResetSceneTable" function
constexpr chip::FabricIndex kFabric1 = 1;
constexpr chip::FabricIndex kFabric2 = 7;

// Scene storage ID
static const SceneStorageId sceneId1(1, 0xAA, 0x101);
static const SceneStorageId sceneId2(1, 0xBB, 0x00);
static const SceneStorageId sceneId3(1, 0xCC, 0x102);
static const SceneStorageId sceneId4(1, 0xBE, 0x00);
static const SceneStorageId sceneId5(1, 0x45, 0x103);
static const SceneStorageId sceneId6(1, 0x65, 0x00);
static const SceneStorageId sceneId7(1, 0x77, 0x101);
static const SceneStorageId sceneId8(1, 0xEE, 0x101);
static const SceneStorageId sceneId9(1, 0xAB, 0x101);

// Scene data
static const SceneData sceneData1("Scene #1");
static const SceneData sceneData2("Scene #2", 2, 5);
static const SceneData sceneData3("Scene #3", 25);
static const SceneData sceneData4("Scene num4", 5);
static const SceneData sceneData5(nullptr);
static const SceneData sceneData6("Scene #6", 3, 15);
static const SceneData sceneData7("Scene #7", 20, 5);
static const SceneData sceneData8("Scene num8", 10);
static const SceneData sceneData9("NAME TOO LOOONNG", 30, 15);
static const SceneData sceneData10("Scene #10", 10, 1);
static const SceneData sceneData11("Scene #11", 20, 10);
static const SceneData sceneData12("Scene #12", 30, 5);

// Scenes
SceneTableEntry scene1(sceneId1, sceneData1);
SceneTableEntry scene2(sceneId2, sceneData2);
SceneTableEntry scene3(sceneId3, sceneData3);
SceneTableEntry scene4(sceneId4, sceneData4);
SceneTableEntry scene5(sceneId5, sceneData5);
SceneTableEntry scene6(sceneId6, sceneData6);
SceneTableEntry scene7(sceneId7, sceneData7);
SceneTableEntry scene8(sceneId8, sceneData8);
SceneTableEntry scene9(sceneId9, sceneData9);
SceneTableEntry scene10(sceneId1, sceneData10);
SceneTableEntry scene11(sceneId5, sceneData11);
SceneTableEntry scene12(sceneId8, sceneData12);

void ResetSceneTable(SceneTableImpl * sceneTable)
{
    sceneTable->RemoveFabric(kFabric1);
    sceneTable->RemoveFabric(kFabric2);
}

void TestStoreScenes(nlTestSuite * aSuite, void * aContext)
{
    SceneTableImpl * sceneTable = chip::scenes::GetSceneTable();
    NL_TEST_ASSERT(aSuite, sceneTable);

    // Reset test
    ResetSceneTable(sceneTable);

    SceneTableEntry scene;

    // Set test
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene1));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene2));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene3));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene4));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene5));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene6));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene7));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene8));

    // Too many scenes 1 fabric
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_INVALID_LIST_LENGTH == sceneTable->SetSceneTableEntry(kFabric1, scene9));

    // Not Found
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->SetSceneTableEntry(kFabric1, scene9));

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, scene == scene1);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, scene == scene2);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId3, scene));
    NL_TEST_ASSERT(aSuite, scene == scene3);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId4, scene));
    NL_TEST_ASSERT(aSuite, scene == scene4);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId5, scene));
    NL_TEST_ASSERT(aSuite, scene == scene5);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId6, scene));
    NL_TEST_ASSERT(aSuite, scene == scene6);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId7, scene));
    NL_TEST_ASSERT(aSuite, scene == scene7);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId8, scene));
    NL_TEST_ASSERT(aSuite, scene == scene8);
}

void TestOverwriteScenes(nlTestSuite * aSuite, void * aContext)
{
    SceneTableImpl * sceneTable = chip::scenes::GetSceneTable();
    NL_TEST_ASSERT(aSuite, sceneTable);

    SceneTableEntry scene;
    // Overwriting the first entry
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene10));
    // Overwriting in the middle
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene11));
    // Overwriting the last entry
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene12));

    // Scene 10 has the same sceneId as scene 1, Get->sceneId1 should thus return scene 10, etc.
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, scene == scene10);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId5, scene));
    NL_TEST_ASSERT(aSuite, scene == scene11);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId8, scene));
    NL_TEST_ASSERT(aSuite, scene == scene12);
}

void TestIterateScenes(nlTestSuite * aSuite, void * aContext)
{
    SceneTableImpl * sceneTable = chip::scenes::GetSceneTable();
    NL_TEST_ASSERT(aSuite, sceneTable);

    SceneTableEntry scene;
    auto * iterator = sceneTable->IterateSceneEntry(kFabric1);

    if (iterator)
    {
        NL_TEST_ASSERT(aSuite, iterator->Count() == 8);

        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene10);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene2);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene3);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene4);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene11);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene6);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene7);
        NL_TEST_ASSERT(aSuite, iterator->Next(scene));
        NL_TEST_ASSERT(aSuite, scene == scene12);

        NL_TEST_ASSERT(aSuite, iterator->Next(scene) == false);

        iterator->Release();
    }
}

void TestRemoveScenes(nlTestSuite * aSuite, void * aContext)
{
    SceneTableImpl * sceneTable = chip::scenes::GetSceneTable();
    NL_TEST_ASSERT(aSuite, sceneTable);

    SceneTableEntry scene;

    // Remove middle
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene5.storageId));
    auto * iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 7);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene1);
    iterator->Release();

    // Remove first
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene1.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 6);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene2);

    // Remove Next
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene3.storageId));
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene4);

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene2.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 4);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene4);

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene4.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 3);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene6);

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene6.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 2);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene7);

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene7.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 2);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene));
    NL_TEST_ASSERT(aSuite, scene == scene8);

    // Remove last
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveSceneTableEntry(kFabric1, scene8.storageId));
    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 0);
    NL_TEST_ASSERT(aSuite, iterator->Next(scene) == false);

    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->RemoveSceneTableEntry(kFabric1, scene8.storageId));

    iterator = sceneTable->IterateSceneEntry(kFabric1);
    NL_TEST_ASSERT(aSuite, iterator->Count() == 0);
    auto * iterator = sceneTable->IterateSceneEntry(kFabric1);
}

void TestFabricScenes(nlTestSuite * aSuite, void * aContext)
{
    SceneTableImpl * sceneTable = chip::scenes::GetSceneTable();
    NL_TEST_ASSERT(aSuite, sceneTable);

    // Reset test
    ResetSceneTable(sceneTable);

    SceneTableEntry scene;

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene1));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene2));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric1, scene3));

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric2, scene1));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric2, scene2));
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->SetSceneTableEntry(kFabric2, scene3));

    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, scene == scene1);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, scene == scene2);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric1, sceneId3, scene));
    NL_TEST_ASSERT(aSuite, scene == scene3);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, scene == scene1);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, scene == scene2);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId3, scene));
    NL_TEST_ASSERT(aSuite, scene == scene3);

    // Remove Fabric 1
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveFabric(kFabric1));
    // Verify Fabric 1 removed
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->RemoveFabric(kFabric1));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric1, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric1, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric1, sceneId3, scene));

    // Confirm Fabric 2 still there
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, scene == scene1);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, scene == scene2);
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->GetSceneTableEntry(kFabric2, sceneId3, scene));
    NL_TEST_ASSERT(aSuite, scene == scene3);

    // Remove Fabric 2
    NL_TEST_ASSERT(aSuite, CHIP_NO_ERROR == sceneTable->RemoveFabric(kFabric2));
    // Verify Fabric 1 removed
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->RemoveFabric(kFabric2));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric2, sceneId1, scene));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric2, sceneId2, scene));
    NL_TEST_ASSERT(aSuite, CHIP_ERROR_NOT_FOUND == sceneTable->GetSceneTableEntry(kFabric2, sceneId3, scene));
}

} // namespace

int TestSetup(void * inContext)
{
    VerifyOrReturnError(CHIP_NO_ERROR == chip::Platform::MemoryInit(), FAILURE);
    VerifyOrReturnError(CHIP_NO_ERROR == sSceneTable.Init(&testStorage), FAILURE);

    SetSceneTable(&sSceneTable);
}

int TestSceneTable()
{
    static nlTest sTests[] = {
        NL_TEST_DEF("TestFabricScenes", TestFabricScenes),       NL_TEST_DEF("TestStoreScenes", TestStoreScenes),
        NL_TEST_DEF("TestOverwriteScenes", TestOverwriteScenes), NL_TEST_DEF("TestIterateScenes", TestIterateScenes),
        NL_TEST_DEF("TestRemoveScenes", TestRemoveScenes),       NL_TEST_SENTINEL()
    };

    nlTestSuite theSuite = {
        "SceneTable",
        &sTests[0],
        nullptr,
        nullptr,
    };
    nlTestRunner(&theSuite, nullptr);
    return (nlTestRunnerStats(&theSuite));
}

CHIP_REGISTER_TEST_SUITE(TestSceneTable)