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
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h> // for execlp
#include <mqueue.h> // for mq

#include "settings.h" // definition of work
#include "common.h"

static char mq_name1[80]; // definition of the message queque name
static char mq_name2[80];

#define STUDENT_NAME "TEST" // just added as a test, needs to be changed later

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "%s: invalid arguments\n", argv[0]);
    }

    // TODO:
    //  * create the message queues (see message_queue_test() in
    //    interprocess_basic.c)

    pid_t processID[NROF_WORKERS]; /* Array of proces ID from fork() */

    // i took the following code from interproces_basics.c
    mqd_t mq_fd_request;
    mqd_t mq_fd_response;
    MQ_REQUEST_MESSAGE req;
    MQ_RESPONSE_MESSAGE rsp;
    struct mq_attr attr;

    sprintf(mq_name1, "/mq_request_%s_%d", STUDENT_NAME, getpid());
    sprintf(mq_name2, "/mq_response_%s_%d", STUDENT_NAME, getpid());

    attr.mq_maxmsg = MQ_MAX_MESSAGES;
    attr.mq_msgsize = sizeof(MQ_REQUEST_MESSAGE);
    mq_fd_request = mq_open(mq_name1, O_WRONLY | O_CREAT | O_EXCL, 0600, &attr);

    attr.mq_maxmsg = MQ_MAX_MESSAGES;
    attr.mq_msgsize = sizeof(MQ_RESPONSE_MESSAGE);
    mq_fd_response = mq_open(mq_name2, O_RDONLY | O_CREAT | O_EXCL, 0600, &attr);

    //  * create the child processes (see process_test() and
    //    message_queue_test())

    //  * do the farming

    //  * wait until the chilren have been stopped (see process_test())

    //  * clean up the message queues (see message_queue_test())

    mq_close(mq_fd_response);
    mq_close(mq_fd_request);
    mq_unlink(mq_name1);
    mq_unlink(mq_name2);

    // Important notice: make sure that the names of the message queues
    // contain your student name and the process id (to ensure uniqueness
    // during testing)

    return (0);
}
