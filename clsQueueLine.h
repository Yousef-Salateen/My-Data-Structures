#pragma once
#include "IclsLine.h"
#include "../../../header files/clsDate.h"
#include <string>
#include <stdexcept>

class clsQueueLine : public IclsLine
{
private:
    class QueueTicket : public IclsLine::ITicket
    {
    private:
        const std::string _Number;
        const clsDate _Date;
        const int _ClientsWaiting;
        const long long _TimeToServe;

    public:
        QueueTicket(const std::string& number, const clsDate& date,
            int waiting, long long timeToServe)
            : _Number(number), _Date(date),
            _ClientsWaiting(waiting), _TimeToServe(timeToServe)
        {
        }

        ~QueueTicket() override = default;

        const std::string& Number() const override
        {
            return _Number;
        }

        std::string DateString(const std::string& Format) const override
        {
            return clsDate::FormatDate(_Date, Format);
        }

        const clsDate& Date() const
        {
            return _Date;
        }

        int Waiting() const override
        {
            return _ClientsWaiting;
        }

        long long TimeToServe() const override
        {
            return _TimeToServe;
        }
    };

    const std::string _Prefix;
    const int _WaitingTime;
    int _ServedClients;
    std::queue<ITicket*> _Line;

    const std::string& Prefix() const
    {
        return _Prefix;
    }

    int WaitingTime() const
    {
        return _WaitingTime;
    }

public:
    clsQueueLine(const std::string& Prefix, int WaitingTime)
        : _Prefix(Prefix), _WaitingTime(WaitingTime), _ServedClients(0)
    {
    }

    ~clsQueueLine() override
    {
        while (!_Line.empty())
        {
            delete _Line.front();
            _Line.pop();
        }
    }

    void IssueTicket() override
    {
        QueueTicket* Ticket = new QueueTicket(
            Prefix() + std::to_string(TotalTickets() + 1),
            clsDate(),
            WaitingClients(),
            static_cast<long long>(WaitingTime()) * WaitingClients()
        );

        _Line.push(Ticket);
    }

    void ServeNextClient() override
    {
        if (_Line.empty())
            throw std::runtime_error("Serving from an empty queue.");

        _ServedClients++;

        delete _Line.front();
        _Line.pop();
    }

    int TotalTickets() const override
    {
        return ServedClients() + static_cast<int>(_Line.size());
    }

    int ServedClients() const override
    {
        return _ServedClients;
    }

    int WaitingClients() const override
    {
        return static_cast<int>(_Line.size());
    }
    void ForEachTicket(void(*Action)(const ITicket& Ticket)) const override
    {
        std::queue<ITicket*> Copy = _Line;

        while (!Copy.empty())
        {
            Action(*Copy.front());
            Copy.pop();
        }
    }

};