/*
 * lftp and utils
 *
 * Copyright (c) 1996-1997 by Alexander V. Lukyanov (lav@yars.free.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef QUEUEFEEDER_H
#define QUEUEFEEDER_H

#include "CmdExec.h"

class QueueFeeder : public CmdFeeder
{
   struct QueueJob {
      char *cmd;
      char *pwd, *lpwd;

      QueueJob *next, *prev;
      QueueJob(): cmd(0), pwd(0), lpwd(0), next(0), prev(0) { }
      ~QueueJob();
   } *jobs, *lastjob;
   char *cur_pwd, *cur_lpwd;

   char *buffer;

   /* remove the given job from the list */
   void unlink_job(QueueJob *job);
      
   /* get the n'th job */
   QueueJob *get_job(int n);
      
   /* get the n'th job, removed from the list: */
   QueueJob *grab_job(int n);

   /* get all jobs (linked and removed from the list)
    * that match the cmd: */
   QueueJob *grab_job(const char *cmd);

   void insert_jobs(QueueJob *job,
		   QueueJob *&lst_head, QueueJob *&lst_tail,
		   QueueJob *before);
      
   void FreeList(QueueJob *j);

public:
   const char *NextCmd(CmdExec *exec,const char *prompt);

   /* Add a command to the queue at a given position; a 0 position inserts at the end. */
   void QueueCmd(const char *cmd, const char *pwd, const char *lpwd, int pos = 0);

   /* delete jobs (by index or wildcard expr) */
   bool DelJob(int from);
   bool DelJob(const char *cmd);

   /* move one or more jobs (by index or wildcard expr). */
   bool MoveJob(int from, int to);
   bool MoveJob(const char *cmd, int to);

   void PrintStatus(int v) const;

   QueueFeeder(const char *pwd, const char *lpwd):
      jobs(0), lastjob(0), cur_pwd(0), cur_lpwd(0), buffer(0)
   {
      cur_pwd = xstrdup(pwd);
      cur_lpwd = xstrdup(lpwd);
   }
   virtual ~QueueFeeder();
};

#endif//QUEUEFEEDER_H