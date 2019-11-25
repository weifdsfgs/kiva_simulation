#ifndef KIVA_SIMULATION_SRC_KS_SCHEDULER_COMMON_H_
#define KIVA_SIMULATION_SRC_KS_SCHEDULER_COMMON_H_

#include <mutex>

#include <iostream>

#include "constants.h"
#include "utilities.h"
#include "interface/wms_scheduler_types.h"

namespace ks {
struct InternalMission {
  // Internal mission, for a robot without shelf moving around.
  Location to;
};

struct Mission {
  bool is_internal;
  WmsMission wms_mission;
  InternalMission internal_mission;
  Mission() = default;
  Mission &operator=(const Mission &o) = default;
};

struct Position {
  Location loc;
  Direction dir;

  Position() = default;
  Position(int x, int y, Direction dir) : loc(x, y), dir(dir) {};
  Position(Location loc, Direction dir) : loc(loc), dir(dir) {};

  bool operator<(const Position &o) const {
    if (loc != o.loc) {
      return loc < o.loc;
    }
    return dir < o.dir;
  }

  bool operator==(const Position &o) const {
    return loc == o.loc && dir == o.dir;
  }

  bool operator!=(const Position &o) const {
    return !operator==(o);
  }

  std::string to_string() const {
    return loc.to_string() + " " + kDirectionToString.at(dir);
  }
};

struct RobotInfo {
  int id;
  Position pos;
  bool shelf_attached;

  // When has_mission is false, and report_mission_done is true, the robot has
  // finished the mission, and the scheduler needs to update this to wms.
  bool has_mission;
  Mission mission;

  RobotInfo(int id, Location loc) : id(id) {
    pos = {loc, Direction::NORTH};
    shelf_attached = false;
    has_mission = false;
  }

  RobotInfo(const RobotInfo &o) = default;
  RobotInfo &operator=(const RobotInfo &o) = default;

  bool IsIdle() const {
    bool rtn = !has_mission;
    if (rtn) {
      assert(!shelf_attached);
    }
    return rtn;
  }

  std::string to_string() const {
    return pos.to_string();
  }
};

// TODO: think if the shelf manager can be updated here.
inline void ApplyActionOnRobot(Action a, RobotInfo *r) {
  switch (a) {
    case Action::ATTACH:assert(r->shelf_attached == false);
      r->shelf_attached = true;
      break;
    case Action::DETACH:assert(r->shelf_attached == true);
      r->shelf_attached = false;
      r->has_mission = false;
      break;
    case Action::YIELD:
      // Yield is effectively a WAIT operation in the MAPF setting. Do nothing.
      break;
    case Action::MOVE:r->pos.loc = r->pos.loc + kDirectionToDelta.at(r->pos.dir);
      break;
    case Action::CTURN:r->pos.dir = ClockwiseTurn(r->pos.dir);
      break;
    case Action::CCTURN:r->pos.dir = CounterClockwiseTurn(r->pos.dir);
      break;
    default:exit(0);
  }
}

inline Position ApplyActionOnPosition(Action a, Position p) {
  Position rtn = p;
  switch (a) {
    case Action::ATTACH:break;
    case Action::DETACH:break;
    case Action::YIELD:break;
    case Action::MOVE:rtn.loc = rtn.loc + kDirectionToDelta.at(rtn.dir);
      break;
    case Action::CTURN:rtn.dir = ClockwiseTurn(rtn.dir);
      break;
    case Action::CCTURN:rtn.dir = CounterClockwiseTurn(rtn.dir);
      break;
    default:exit(0);
  }
  return rtn;
}

struct ActionWithTime {
  Action action;
  int start_time_ms;
  int end_time_ms;
  Position start_pos;
  Position end_pos;

  ActionWithTime() = default;
  ActionWithTime(Action action, int start_time_ms, int end_time_ms, Position start_pos, Position end_pos)
      : action(action), start_time_ms(start_time_ms), end_time_ms(end_time_ms),
        start_pos(start_pos), end_pos(end_pos) {};

  std::string to_string() const {
    return kActionToString.at(action)
    + " start: " + std::to_string(start_time_ms)
    + " end: " + std::to_string(end_time_ms);
  }
};

using ActionWithTimeSeq = std::vector<ActionWithTime>;

class ShelfManager {
 public:
  ShelfManager() = default;
  ShelfManager(const ShelfManager& o) {
    loc_to_id_ = o.loc_to_id_;
  }

  void AddMapping(int shelf_id, Location loc) {
//    std::cout << "Add mapping: shelf id: " << shelf_id << " location: " << loc.to_string() << std::endl;
    assert(loc_to_id_.find(loc) == loc_to_id_.end());
    loc_to_id_[loc] = shelf_id;
  }

  void RemoveMapping(int shelf_id, Location loc) {
//    std::cout << "Remove mapping: shelf id: " << shelf_id << " location: " << loc.to_string() << std::endl;
    assert(loc_to_id_.find(loc) != loc_to_id_.end());
    assert(loc_to_id_[loc] == shelf_id);
    loc_to_id_.erase(loc);
  }

  bool HasShelf(Location loc) {
    return loc_to_id_.find(loc) != loc_to_id_.end();
  }

  std::map<Location, int> loc_to_id_;
};

}
#endif