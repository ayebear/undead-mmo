#ifndef PACKET_H
#define PACKET_H

// this class will store useful information, by extracting it from a string
// could also do the reverse maybe.
// this could be used for having different packet "types" that all mean different things, like a character coordinate update, or new zombies or items

enum PacketType
{
    ChatMessage = 1,
    EntityUpdate, // this would be pretty advanced, as not all of the data would need to be sent.
    // existing values in RAM will be used in place of non-existant data from the packets.
    // newly spawned entities with new IDs

};

class Packet
{
    public:
        Packet();
    private:
};

#endif // PACKET_H
