#include "sipp_solver.h"

#include "common_types.h"
#include "ks_map.h"
#include "gtest/gtest.h"
#include "ks_scheduler_common.h"

using namespace std;
using namespace ks;

namespace {
class Basic : public ::testing::Test {
 protected:
  Basic() {
  }

  ~Basic() override {
  }

  void SetUp() override {
  }

  void TearDown() override {
  }
};

//TEST_F(Basic, Basic0) {
//  KsMap ks_map("../data/test_map_1.map");
//  ShelfManager shelf_manager;
//  SippSolver solver(ks_map, &shelf_manager);
//
//  RobotInfo robot(0, {0, 0});
//  robot.has_mission = true;
//  robot.mission.is_internal = true;
//  robot.mission.internal_mission.to = {0, 10};
//  PfRequest req;
//  req.robots.push_back(robot);
//  PfResponse resp = solver.FindPath(req);
//
//  for (ActionWithTimeSeq ac : resp.plan) {
//    for (ActionWithTime a : ac) {
//      cout << "action: " + kActionToString.at(a.action)
//           << " start: " << a.start_time << " end: " << a.end_time << endl;
//    }
//  }
//}
//
//TEST_F(Basic, Basic1) {
//  KsMap ks_map(kMapFileName);
//  ShelfManager shelf_manager;
//  SippSolver solver(ks_map, &shelf_manager);
//
//  RobotInfo robot0(0, {0, 0});
//  robot0.has_mission = true;
//  robot0.mission.is_internal = true;
//  robot0.mission.internal_mission.to = {0, 4};
//  RobotInfo robot1(1, {0, 5});
//  robot1.has_mission = true;
//  robot1.mission.is_internal = true;
//  robot1.mission.internal_mission.to = {0, 0};
//
//  PfRequest req;
//  req.robots.push_back(robot0);
//  req.robots.push_back(robot1);
//  PfResponse resp = solver.FindPath(req);
//
//  for (int i = 0; i < resp.plan.size(); i++) {
//    ActionWithTimeSeq &ac = resp.plan[i];
//    cout << "robot id: " << i << endl;
//    for (ActionWithTime a : ac) {
//      cout << "action: " + kActionToString.at(a.action)
//           << " start: " << a.start_time << " end: " << a.end_time << endl;
//    }
//    cout << endl;
//  }
//}

TEST_F(Basic, Basic2) {
  KsMap ks_map(kMapFileName);
  ShelfManager shelf_manager;
  SippSolver solver(ks_map, &shelf_manager);

  RobotInfo robot0(0, {0, 0});
  robot0.pos.dir = Direction::EAST;
  robot0.has_mission = true;
  robot0.mission.is_internal = true;
  robot0.mission.internal_mission.to = {0, 4};
  RobotInfo robot1(1, {0, 5});
  robot1.pos.dir = Direction::WEST;
  robot1.has_mission = true;
  robot1.mission.is_internal = true;
  robot1.mission.internal_mission.to = {0, 0};

  PfRequest req;
  req.robots.push_back(robot0);
  req.robots.push_back(robot1);
  PfResponse resp = solver.FindPath(req);

  for (int i = 0; i < resp.plan.size(); i++) {
    ActionWithTimeSeq &ac = resp.plan[i];
    cout << "robot id: " << i << endl;
    for (ActionWithTime a : ac) {
      cout << "action: " + kActionToString.at(a.action)
           << " start: " << a.start_time << " end: " << a.end_time << endl;
    }
    cout << endl;
  }
}

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}