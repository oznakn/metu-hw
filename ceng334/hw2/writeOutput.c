#include "writeOutput.h"
pthread_mutex_t mutexWrite = PTHREAD_MUTEX_INITIALIZER;

struct timeval startTime;

void InitWriteOutput()
{
    gettimeofday(&startTime, NULL);
}

unsigned long long GetTimestamp()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (currentTime.tv_sec - startTime.tv_sec) * 1000 // second -> millisecond
           + (currentTime.tv_usec - startTime.tv_usec) / 1000; // micro second -> millisecond
}

void PrintThreadId()
{
    pthread_t tid = pthread_self();
    size_t i;
    printf("ThreadID: ");
    for (i=0; i<sizeof(pthread_t); ++i)
        printf("%02x", *(((unsigned char*)&tid) + i));
    printf(", ");
}


void FillPacketInfo(PackageInfo *packageInfo, int sender_id, int sending_hub_id, int receiver_id, int receiving_hub_id) {
    packageInfo->sender_id          = sender_id;
    packageInfo->sending_hub_id     = sending_hub_id;
    packageInfo->receiver_id        = receiver_id;
    packageInfo->receiving_hub_id   = receiving_hub_id;
}

void FillSenderInfo(SenderInfo *senderInfo, int id, int current_hub_id, int remaining_package_count, PackageInfo* packageInfo) {
    senderInfo->id                      = id;
    senderInfo->current_hub_id          = current_hub_id;
    senderInfo->remaining_package_count = remaining_package_count;
    senderInfo->packageInfo             = packageInfo;
}

void FillReceiverInfo(ReceiverInfo *receiverInfo, int id, int current_hub_id, PackageInfo* packageInfo) {
    receiverInfo->id                = id;
    receiverInfo->current_hub_id    = current_hub_id;
    receiverInfo->packageInfo       = packageInfo;
}

void FillDroneInfo(DroneInfo *droneInfo, int id, int current_hub_id, int current_range, PackageInfo* packageInfo, int next_hub_id) {
    droneInfo->id                = id;
    droneInfo->current_hub_id    = current_hub_id;
    droneInfo->current_range     = current_range;
    droneInfo->packageInfo       = packageInfo;
    droneInfo->next_hub_id       = next_hub_id;
}

void FillHubInfo(HubInfo *hubInfo, int id) {
    hubInfo->id = id;
}

void WriteOutput(SenderInfo* senderInfo, ReceiverInfo* receiverInfo, DroneInfo* droneInfo, HubInfo* hubInfo, Action action) {
    unsigned long long time = GetTimestamp();
    unsigned int senderID =0, receiverID=0, droneID=0, hubID=0;
    pthread_mutex_lock(&mutexWrite);

    if ( senderInfo )
        senderID = senderInfo->id;
    if ( receiverInfo )
        receiverID = receiverInfo->id;
    if ( droneInfo )
        droneID = droneInfo->id;
    if ( hubInfo )
        hubID = hubInfo->id;

    PrintThreadId();
    printf("AID: %d SID: %d RID: %d DID: %d HID: %d time stamp: %llu ",  (int)action, senderID, receiverID,
            droneID, hubID, time);
    switch (action) {
        case SENDER_CREATED:
        case SENDER_STOPPED:
            if ( !senderInfo ) {
                printf("\nErroneous output. SenderInfo is null.\n");
                break;
            }
            printf("CHID: %d RPC: %d\n", senderInfo->current_hub_id, senderInfo->remaining_package_count);
            break;
        case SENDER_DEPOSITED:
            if ( !senderInfo ) {
                printf("\nErroneous output. SenderInfo is null.\n");
                break;
            }
            printf("CHID: %d RPC: %d PRID: %d PRHID: %d\n", senderInfo->current_hub_id,
                    senderInfo->remaining_package_count, senderInfo->packageInfo->receiver_id,
                    senderInfo->packageInfo->receiving_hub_id);
            break;
        case RECEIVER_CREATED:
        case RECEIVER_STOPPED:
            if ( !receiverInfo ) {
                printf("Erroneous output. ReceiverInfo is null.\n");
                break;
            }
            printf("CHID: %d\n", receiverInfo->current_hub_id);
            break;
        case RECEIVER_PICKUP:
            if ( !receiverInfo ) {
                printf("\nErroneous output. ReceiverInfo is null.\n");
                break;
            }
            printf("CHID: %d PSID: %d PSHID: %d\n", receiverInfo->current_hub_id,
                    receiverInfo->packageInfo->sender_id, receiverInfo->packageInfo->sending_hub_id);
            break;
        case DRONE_CREATED:
        case DRONE_STOPPED:
            if ( !droneInfo ) {
                printf("\nErroneous output. DroneInfo is null.\n");
                break;
            }
            printf("CHID: %d CR: %d\n", droneInfo->current_hub_id, droneInfo->current_range);
            break;
        case DRONE_PICKUP:
            if ( !droneInfo ) {
                printf("\nErroneous output. DroneInfo is null.\n");
                break;
            }
            printf("CHID: %d CR: %d PSID: %d PSHID: %d\n", droneInfo->current_hub_id, droneInfo->current_range,
                    droneInfo->packageInfo->sender_id, droneInfo->packageInfo->sending_hub_id);
            break;
        case DRONE_DEPOSITED:
            if ( !droneInfo ) {
                printf("\nErroneous output. DroneInfo is null.\n");
                break;
            }
            printf("CHID: %d CR: %d PRID: %d PRHID: %d\n", droneInfo->current_hub_id, droneInfo->current_range,
                   droneInfo->packageInfo->receiver_id, droneInfo->packageInfo->receiving_hub_id);
            break;
        case DRONE_GOING:
            if ( !droneInfo ) {
                printf("\nErroneous output. DroneInfo is null.\n");
                break;
            }
            printf("CHID: %d CR: %d NHID: %d\n", droneInfo->current_hub_id, droneInfo->current_range,
                    droneInfo->next_hub_id);
            break;
        case DRONE_ARRIVED:
            if ( !droneInfo ) {
                printf("\nErroneous output. DroneInfo is null.\n");
                break;
            }
            printf("CHID: %d CR: %d\n", droneInfo->current_hub_id, droneInfo->current_range);
            break;
        case HUB_CREATED:
        case HUB_STOPPED:
            if ( !hubInfo ) {
                printf("\nErroneous output. HubInfo is null.\n");
                break;
            }
            printf("\n");
            break;
        default:
            printf("\nErroneous output.\n");
            break;
    }
    pthread_mutex_unlock(&mutexWrite);
}

