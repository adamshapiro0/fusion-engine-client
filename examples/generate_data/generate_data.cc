/**************************************************************************/ /**
* @brief Message encode example.
******************************************************************************/

#include <cstdio>
#include <fstream>

#include <point_one/fusion_engine/messages/core.h>
#include <point_one/fusion_engine/messages/crc.h>

using namespace point_one::fusion_engine::messages;

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s FILE\n", argv[0]);
    printf(R"EOF(
Generate a binary file containing a fixed set of messages.
)EOF");
    return 0;
  }

  std::ofstream stream(argv[1], std::ifstream::binary);
  if (!stream) {
    printf("Error opening file '%s'.\n", argv[1]);
    return 1;
  }

  // Enforce a 4-byte aligned address.
  uint8_t storage[4096];
  size_t alignment_offset = reinterpret_cast<size_t>(storage) % 4;
  char* buffer = reinterpret_cast<char*>(storage + alignment_offset);

  //////////////////////////////////////////////////////////////////////////////
  // Write a pose message.
  //////////////////////////////////////////////////////////////////////////////

  MessageHeader* header = reinterpret_cast<MessageHeader*>(buffer);
  *header = MessageHeader();
  header->message_type = MessageType::POSE;
  header->payload_size_bytes = sizeof(PoseMessage);

  PoseMessage* pose_message = reinterpret_cast<PoseMessage*>(header + 1);
  *pose_message = PoseMessage();

  pose_message->p1_time.seconds = 123;
  pose_message->p1_time.fraction_ns = 456000000;

  pose_message->gps_time.seconds = 1282677727;
  pose_message->gps_time.fraction_ns = 200000000;

  pose_message->solution_type = SolutionType::RTKFixed;
  pose_message->lla_deg[0] = 37.795137;
  pose_message->lla_deg[1] = -122.402754;
  pose_message->lla_deg[2] = 40.8;

  pose_message->ypr_deg[0] = 190.0;
  pose_message->ypr_deg[1] = 2.1;
  pose_message->ypr_deg[2] = 0.1;

  pose_message->velocity_enu_mps[0] = -2.3;
  pose_message->velocity_enu_mps[1] = -1.5;
  pose_message->velocity_enu_mps[2] = 0.3;

  pose_message->position_std_dev_ecef_m[0] = 0.1;
  pose_message->position_std_dev_ecef_m[1] = 0.1;
  pose_message->position_std_dev_ecef_m[2] = 0.1;

  pose_message->aggregate_protection_level_m = 0.4;
  pose_message->horizontal_protection_level_m = 0.2;
  pose_message->vertical_protection_level_m = 0.3;

  header->crc = CalculateCRC(buffer);
  stream.write(reinterpret_cast<char*>(buffer),
               sizeof(MessageHeader) + header->payload_size_bytes);

  //////////////////////////////////////////////////////////////////////////////
  // Write a GNSS info message associated with the pose message.
  //////////////////////////////////////////////////////////////////////////////

  header = reinterpret_cast<MessageHeader*>(buffer);
  *header = MessageHeader();

  header->message_type = MessageType::GNSS_INFO;
  header->payload_size_bytes =
      sizeof(GNSSInfoMessage) + 2 * sizeof(SatelliteInfo);

  GNSSInfoMessage* gnss_info_message =
      reinterpret_cast<GNSSInfoMessage*>(header + 1);
  *gnss_info_message = GNSSInfoMessage();

  gnss_info_message->p1_time.seconds = 123;
  gnss_info_message->p1_time.fraction_ns = 456000000;

  gnss_info_message->gps_time.seconds = 1282677727;
  gnss_info_message->gps_time.fraction_ns = 200000000;

  gnss_info_message->last_differential_time.seconds = 1282677727;
  gnss_info_message->last_differential_time.fraction_ns = 200000000;

  gnss_info_message->reference_station_id = 4321;

  gnss_info_message->gdop = 1.6;
  gnss_info_message->pdop = 1.3;
  gnss_info_message->hdop = 1.2;
  gnss_info_message->vdop = 1.5;

  gnss_info_message->num_satellites = 2;

  SatelliteInfo* satellite_info =
      reinterpret_cast<SatelliteInfo*>(gnss_info_message + 1);
  *satellite_info = SatelliteInfo();
  satellite_info->system = SatelliteType::GPS;
  satellite_info->prn = 4;
  satellite_info->used_in_solution = 1;
  satellite_info->azimuth_deg = 34.5;
  satellite_info->elevation_deg = 56.2;

  ++satellite_info;
  *satellite_info = SatelliteInfo();
  satellite_info->system = SatelliteType::GALILEO;
  satellite_info->prn = 9;
  satellite_info->used_in_solution = 0;
  satellite_info->azimuth_deg = 79.4;
  satellite_info->elevation_deg = 16.1;

  header->crc = CalculateCRC(buffer);
  stream.write(reinterpret_cast<char*>(buffer),
               sizeof(MessageHeader) + header->payload_size_bytes);

  //////////////////////////////////////////////////////////////////////////////
  // Write another pose message 0.2 seconds later.
  //////////////////////////////////////////////////////////////////////////////

  header = reinterpret_cast<MessageHeader*>(buffer);
  *header = MessageHeader();
  header->message_type = MessageType::POSE;
  header->payload_size_bytes = sizeof(PoseMessage);

  pose_message = reinterpret_cast<PoseMessage*>(header + 1);
  *pose_message = PoseMessage();

  pose_message->p1_time.seconds = 123;
  pose_message->p1_time.fraction_ns = 667000000;

  pose_message->gps_time.seconds = 1282677727;
  pose_message->gps_time.fraction_ns = 400000000;

  pose_message->solution_type = SolutionType::RTKFloat;
  pose_message->lla_deg[0] = 37.802369;
  pose_message->lla_deg[1] = -122.405823;
  pose_message->lla_deg[2] = 82.0;

  pose_message->ypr_deg[0] = 37.0;
  pose_message->ypr_deg[1] = 0.0;
  pose_message->ypr_deg[2] = 0.0;

  pose_message->velocity_enu_mps[0] = 0.0;
  pose_message->velocity_enu_mps[1] = 0.0;
  pose_message->velocity_enu_mps[2] = 1.2;

  pose_message->position_std_dev_ecef_m[0] = 0.05;
  pose_message->position_std_dev_ecef_m[1] = 0.05;
  pose_message->position_std_dev_ecef_m[2] = 0.05;

  pose_message->aggregate_protection_level_m = 0.3;
  pose_message->horizontal_protection_level_m = 0.08;
  pose_message->vertical_protection_level_m = 0.2;

  header->crc = CalculateCRC(buffer);
  stream.write(reinterpret_cast<char*>(buffer),
               sizeof(MessageHeader) + header->payload_size_bytes);

  stream.close();

  return 0;
}
