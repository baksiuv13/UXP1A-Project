#!/bin/bash

ipc_file="$1"

if [ -f "$ipc_file" ] ; then true ;
else echo "${ipc_file} is not a file" ; exit 1 ; fi ;

proj_id_shm=1
proj_id_sem=2

dev=`stat --format=%d $ipc_file`
ino=`stat --format=%i $ipc_file`

#echo $dev
#echo $ino

shm_id=$(( ($ino&0xffff)|(($dev&0xff)<<16)|(($proj_id_shm&0xff)<<24) ))
sem_id=$(( ($ino&0xffff)|(($dev&0xff)<<16)|(($proj_id_sem&0xff)<<24) ))

printf -v shm_id '0x%08x' $shm_id
printf -v sem_id '0x%08x' $sem_id

echo $shm_id
echo $sem_id

ipcrm --shmem-key $shm_id
ipcrm --semaphore-key $sem_id
