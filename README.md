# a C Thread pool, create、kill、puase、restart thread through name string.

Work as Middleware lib, export simple interfate to application layer.


Usage, Please refer to the specific case examples in the test:

1.init pool, param is max threads num, call once!

  threadcommander_init(int threadMaxNum)
  
2.create thread

  threadcommander_create(char * pThreadName, void (*start_rtn)(void *), void *arg, int isRun, int bEnableCtrl)
  
3.ctrl thread , work when bEnableCtrl == 1 and start_rtn proc call threadcommander_waitctrl regularly

  int threadcommander_ctrl(char * pThreadName, int isRun)
  
4.kill thread

  int threadcommander_kill(char * pThreadName)
  
  
