#ifndef HOMEWORK2_WRITEOUTPUT_H
#define HOMEWORK2_WRITEOUTPUT_H

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef enum Action {
    SENDER_CREATED,
    SENDER_DEPOSITED,
    SENDER_STOPPED,
    RECEIVER_CREATED,
    RECEIVER_PICKUP,
    RECEIVER_STOPPED,
    DRONE_CREATED,
    DRONE_DEPOSITED,
    DRONE_PICKUP,
    DRONE_GOING,
    DRONE_ARRIVED,
    DRONE_STOPPED,
    HUB_CREATED,
    HUB_STOPPED
}Action;

typedef struct PackageInfo {
    int sender_id;
    int sending_hub_id;
    int receiver_id;
    int receiving_hub_id;
} PackageInfo;

typedef struct SenderInfo {
    int id;
    int current_hub_id;
    int remaining_package_count;
    PackageInfo *packageInfo;
} SenderInfo;

typedef struct ReceiverInfo {
    int id;
    int current_hub_id;
    PackageInfo* packageInfo;
} ReceiverInfo;

typedef struct DroneInfo {
    int id;
    int current_hub_id;
    int current_range;
    PackageInfo* packageInfo;
    int next_hub_id;
} DroneInfo;

typedef struct HubInfo {
    int id;
} HubInfo;

void InitWriteOutput();
unsigned long long GetTimestamp();
void PrintThreadId();

void FillPacketInfo(PackageInfo *packageInfo, int sender_id, int sending_hub_id, int receiver_id, int receiving_hub_id);
void FillSenderInfo(SenderInfo *senderInfo, int id, int current_hub_id, int remaining_package_count, PackageInfo* packageInfo);
void FillReceiverInfo(ReceiverInfo *receiverInfo, int id, int current_hub_id, PackageInfo* packageInfo);
void FillDroneInfo(DroneInfo *droneInfo, int id, int current_hub_id, int current_range, PackageInfo* packageInfo, int next_hub_id);
void FillHubInfo(HubInfo *hubInfo, int id);

void WriteOutput(SenderInfo* senderInfo, ReceiverInfo* receiverInfo, DroneInfo* droneInfo, HubInfo* hubInfo, Action action);

#endif //HOMEWORK2_WRITEOUTPUT_H
