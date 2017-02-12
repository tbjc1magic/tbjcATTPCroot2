

#include "FairTask.h"
#include "FairLogger.h"
#include "TCollection.h"                // for TIter
#include "TList.h"                      // for TList
#include "TObject.h"                    // for TObject

#include <iostream>                     // for cout, endl

using std::cout;
using std::endl;

FairTask::FairTask()
    :TTask()
{
}

FairTask::FairTask(const char* name, Int_t iVerbose)
    :TTask(name, "FairTask")
{

}

FairTask::~FairTask() { }

void FairTask::FinishEvent()
{
  if ( ! fActive ) { return; }
  FinishEvents();
  //  FinishTasks();
}

void FairTask::FinishTask()
{
  if ( ! fActive ) { return; }
  Finish();
  FinishTasks();
}

void FairTask::InitTask()
{
  if ( ! fActive ) { return; }
  InitStatus tStat = Init();
//  if ( tStat == kFATAL ) {
//    fLogger->Fatal(MESSAGE_ORIGIN,"Initialization of Task %s failed fatally", fName.Data());
//  }
  if ( tStat == kERROR ) { fActive = kFALSE; }
  InitTasks();
}

void FairTask::InitTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->InitTask(); }
}

void FairTask::SetParTask()
{
  if ( ! fActive ) { return; }
  SetParContainers();
  SetParTasks();
}

void FairTask::SetParTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->SetParTask(); }
}

void FairTask::ExecuteTask(Option_t *option)
{

   if (fgBeginTask) {
      Error("ExecuteTask","Cannot execute task:%s, already running task: %s",GetName(),fgBeginTask->GetName());
      return;
   }
   if (!IsActive()) return;

   fOption = option;
   fgBeginTask = this;
   fgBreakPoint = 0;

   if (fBreakin) return;
   Exec(option);

   fHasExecuted = kTRUE;
   ExecuteTasks(option);

   if (fBreakout) return;

   if (!fgBreakPoint) {
      fgBeginTask->CleanTasks();
      fgBeginTask = 0;
   }
}

void FairTask::ExecuteTasks(Option_t *option)
{
   // Execute all the subtasks of a task.

 //   std::cout<<value<<std::endl;

   TIter next(fTasks);
   FairTask *task;
   while((task=static_cast<FairTask*>(next()))) {
      if (fgBreakPoint) return;
      if (!task->IsActive()) continue;
      if (task->fHasExecuted) {
         task->ExecuteTasks(option);
         continue;
      }
      if (task->fBreakin == 1) {
         printf("Break at entry of task: %s\n",task->GetName());
         fgBreakPoint = this;
         task->fBreakin++;
         return;
      }

      task->Exec(option);

      task->fHasExecuted = kTRUE;
      task->ExecuteTasks(option);
      if (task->fBreakout == 1) {
	printf("Break at exit of task: %s\n",task->GetName());
	fgBreakPoint = this;
	task->fBreakout++;
	return;
      }
   }
}

void FairTask::FinishTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->FinishTask(); }
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->Finish(); }
}

void FairTask::FinishEvents()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->FinishEvent(); }
}
