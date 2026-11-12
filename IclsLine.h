#pragma once
#include <queue>
#include <string>
#include <vector>

class IclsLine
{
public:
    class ITicket
    {
    public:
        virtual ~ITicket() = default;

        virtual const std::string& Number() const = 0;
        virtual std::string DateString(const std::string& Format = "dd-mm-yyyy") const = 0;
        virtual long long TimeToServe() const = 0;
        virtual int Waiting() const = 0;
    };

    virtual ~IclsLine() = default;

    virtual void IssueTicket() = 0;
    virtual void ServeNextClient() = 0;

    virtual int TotalTickets() const = 0;
    virtual int ServedClients() const = 0;
    virtual int WaitingClients() const = 0;
    virtual void ForEachTicket(void(*Action)(const ITicket& Ticket)) const = 0;
};
