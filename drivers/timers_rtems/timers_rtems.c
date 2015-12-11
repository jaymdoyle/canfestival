#include <stdlib.h>

#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include <applicfg.h>
#include <timer.h>

#include <rtems.h>

rtems_id canopen_mutex;
rtems_id canopen_rx_task_id;
rtems_id canopen_timer;
uint32_t last_signal_time_ticks;

void EnterMutex(void) {
  rtems_semaphore_obtain(canopen_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
}

void LeaveMutex(void) {
  rtems_semaphore_release(canopen_mutex);
}

void timer_notify(rtems_id id, void * arg) {
  last_signal_time_ticks = rtems_clock_get_ticks_since_boot();

  EnterMutex();
  TimeDispatch();
  LeaveMutex();
}

void TimerInit(void) {
  rtems_status_code sc;
  sc = rtems_semaphore_create(
      rtems_build_name('C', 'M', 'T', 'X'),
      1,
      RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_PRIORITY,
      0,
      &canopen_mutex);

  sc = rtems_timer_create(
      rtems_build_name('C','T','M','R'),
      &canopen_timer
      );
  sc = rtems_timer_initiate_server(5, RTEMS_MINIMUM_STACK_SIZE, RTEMS_NO_FLOATING_POINT);

}

void StopTimerLoop(TimerCallback_t exitfunction) {
  EnterMutex();
  LeaveMutex();
}

void StartTimerLoop(TimerCallback_t init_callback) {
  EnterMutex();
  // At first, TimeDispatch will call init_callback.
  SetAlarm(NULL, 0, init_callback, 0, 0);
  LeaveMutex();
}

void CreateReceiveTask(CAN_PORT port, TASK_HANDLE* Thread, void* ReceiveLoopPtr) {
  rtems_status_code sc;
  sc = rtems_task_create(
      rtems_build_name('C', 'R', 'X', 'T'), 100,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_PREEMPT,
      RTEMS_FLOATING_POINT, &canopen_rx_task_id);
  if (sc != RTEMS_SUCCESSFUL) {
    fprintf(stderr, "Failed to create rx task\r\n");
  }
  sc = rtems_task_start(
      canopen_rx_task_id,
      ReceiveLoopPtr,
      (rtems_task_argument) port);
  if (sc != RTEMS_SUCCESSFUL) {
     fprintf(stderr, "Failed to start rx task\r\n");
   }
}

void WaitReceiveTaskEnd(TASK_HANDLE *Thread) {
  rtems_task_delete(canopen_rx_task_id);
}

#define maxval(a,b) ((a>b)?a:b)


void setTimer(TIMEVAL value) {
  rtems_status_code sc;
  sc = rtems_timer_server_fire_after(canopen_timer, value + 1, timer_notify, NULL);
}

TIMEVAL getElapsedTime(void) {
  return rtems_clock_get_ticks_per_second() / last_signal_time_ticks;
}
