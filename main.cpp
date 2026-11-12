#include "clsQueueLine.h"
#include <iostream>

static void PrintTickets(const IclsLine& Line)
{
    Line.ForEachTicket([](const IclsLine::ITicket& Ticket)
        {
            std::cout << Ticket.Number() << " ";
            std::cout << Ticket.DateString() << " ";
            std::cout << Ticket.TimeToServe();
            std::cout << std::endl;
        });
}


int main()
{
    clsQueueLine Q("A0", 10);

    Q.IssueTicket();
    Q.IssueTicket();
    Q.IssueTicket();
    Q.IssueTicket();

    PrintTickets(Q);
}