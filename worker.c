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
static MQ_RESPONSE_MESSAGE bruteforce(MQ_REQUEST_MESSAGE req);

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

        // TODO:
        //      - do that job
        rsp = bruteforce(req);

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

static MQ_RESPONSE_MESSAGE bruteforce(MQ_REQUEST_MESSAGE req)
{
    MQ_RESPONSE_MESSAGE rsp;
    char word[MAX_MESSAGE_LENGTH];
    // Initializes the response string
    for (int i = 0; i < MAX_MESSAGE_LENGTH; i++)
    {
        rsp.match[i] = '\0';
        word[i] = '\0';
    }

    word[0] = req.first_letter; //copy first letter in the 'word' string
    uint128_t hash;
    hash = md5s(word, strlen(word)); //calculates the hash of the string

    if (hash == req.md5_hash) // if hash match the word copy the result in response.match
    {
        strcpy(rsp.match, word);
        return rsp;
    }
    else
    {
        return rsp;
    }
}