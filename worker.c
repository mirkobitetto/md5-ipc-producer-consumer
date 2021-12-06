/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * STUDENT_NAME_1 (STUDENT_NR_1)
 * STUDENT_NAME_2 (STUDENT_NR_2)
 *
 * Grading:
 * Your work will be evaluated based on the following criteria:
 * - Satisfaction of all the specifications
 * - Correctness of the program
 * - Coding style
 * - Report quality
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>  // for perror()
#include <unistd.h> // for getpid()
#include <mqueue.h> // for mq-stuff
#include <time.h>   // for time()
#include <complex.h>

#include "common.h"
#include "md5s.h"

struct mq_attr attr;

static void rsleep(int t);

bool calculate_hash(MQ_REQUEST_MESSAGE req, char word[]);
void bruteSequential(MQ_REQUEST_MESSAGE req, MQ_RESPONSE_MESSAGE *rsp);
bool bruteImpl(char *str, int index, int maxDepth, int alphabetSize, MQ_REQUEST_MESSAGE req, MQ_RESPONSE_MESSAGE *rsp);
//

//

int main(int argc, char *argv[])
{

    // (see message_queue_test() in interprocess_basic.c)
    //  * open the two message queues (whose names are provided in the
    //    arguments)
    mqd_t mq_fd_request;
    mqd_t mq_fd_response;
    MQ_REQUEST_MESSAGE req;
    MQ_RESPONSE_MESSAGE rsp;

    mq_fd_request = mq_open(argv[1], O_RDONLY, &attr); //open request message queue with name recived from argument (argv[1]) and initializes the attributes struct
    mq_fd_response = mq_open(argv[2], O_WRONLY);       //open response message queue with name recived from argument (argv[2])

    //  * repeatingly:
    mq_getattr(mq_fd_request, &attr); // get attrubutes of message queues and puts them in attr struct

    while (attr.mq_curmsgs != 0) /* mq_curmsgs is # of messages currently in queue */
    {
        //      - read from a message queue the new job to do
        mq_receive(mq_fd_request, (char *)&req, sizeof(req), NULL);

        //      - wait a random amount of time (e.g. rsleep(10000);)
        rsleep(10000);

        //      - do that job

        // Initializes the response string
        for (int i = 0; i < MAX_MESSAGE_LENGTH + 1; i++)
        {
            rsp.match[i] = '\0';
        }

        char initial[] = {req.first_letter, '\0'}; //let's test if the initial is the solution

        bool found = calculate_hash(req, initial);

        if (found == true)
        {
            strcpy(rsp.match, initial);
        }
        else
        {
            bruteSequential(req, &rsp);
        }

        //      - write the results to a message queue
        mq_send(mq_fd_response, (char *)&rsp, sizeof(rsp), 0);

        // Update attributes
        mq_getattr(mq_fd_request, &attr);
    }
    //    until there are no more tasks to do

    //  * close the message queues
    mq_close(mq_fd_response);
    mq_close(mq_fd_request);

    return (0);
}

/*
 * rsleep(int t)
 *
 * The calling thread will be suspended for a random amount of time
 * between 0 and t microseconds
 * At the first call, the random generator is seeded with the current time
 */
static void rsleep(int t)
{
    static bool first_call = true;

    if (first_call == true)
    {
        srandom(time(NULL) % getpid());
        first_call = false;
    }
    usleep(random() % t);
}

bool calculate_hash(MQ_REQUEST_MESSAGE req, char word[])
{
    uint128_t hash = md5s(word, strlen(word));
    if (hash == req.md5_hash)
    {
        return true;
    }
    return false;
}

void bruteSequential(MQ_REQUEST_MESSAGE req, MQ_RESPONSE_MESSAGE *rsp)
{
    char buff[MAX_MESSAGE_LENGTH + 1];
    int alphabetSize = (int)req.END_CHAR_ALPHABET - (int)req.START_CHAR_ALPHABET + 1;

    bool found = false;
    for (int i = 1; i < MAX_MESSAGE_LENGTH && !found; ++i)
    {
        memset(buff, 0, MAX_MESSAGE_LENGTH + 1);
        buff[0] = req.first_letter;
        found = bruteImpl(buff, 1, i, alphabetSize, req, rsp);
    }
}

bool bruteImpl(char *str, int index, int maxDepth, int alphabetSize, MQ_REQUEST_MESSAGE req, MQ_RESPONSE_MESSAGE *rsp)
{
    for (int i = 0; i < alphabetSize; ++i)
    {
        str[index] = req.START_CHAR_ALPHABET + i;

        if (index == maxDepth)
        { // Changed last letter of the word
            //printf("genword: %s\n", str); // TODO Hash calculation and comparison
            if (calculate_hash(req, str))
            {
                strcpy(rsp->match, str);
                return true;
            }
        }
        else
            bruteImpl(str, index + 1, maxDepth, alphabetSize, req, rsp);
    }
    return false;
}
