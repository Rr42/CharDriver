#!/bin/bash

if [ $# -ge 1 ]; then
    if [ $1 -eq 0 ]; then
        echo "Log dump:"
        dmesg
        exit
    fi
fi

if [ $# -le 1 ]; then
    echo "Provide execution mode"
    echo "Parameter 1:"
    echo "  > 0 : dmesg"
    echo "  > 1 : Load module"
    echo "  > 2 : Unload module"
    echo "  > 3 : Load and unload moule"
    echo "Parameter 2:"
    echo "  > 0 : desable tail"
    echo "  > 1 : enable tail"
elif [ $1 -eq 1 ]; then
    insmod CharDriver.ko
    if [ $? -eq 0 ]; then
        echo "Module loaded"
    else
        if [ $2 -eq 1 ]; then
            tail /var/log/kern.log
        fi
        echo "FAIL"
        exit
    fi
    if [ $2 -eq 1 ]; then
        tail /var/log/kern.log
    fi
elif [ $1 -eq 2 ]; then
    rmmod CharDriver.ko
    if [ $? -eq 0 ]; then
        echo "Module unloaded"
    else
        if [ $2 -eq 1 ]; then
            tail /var/log/kern.log
        fi
        echo "FAIL"
        exit
    fi
    if [ $2 -eq 1 ]; then
        tail /var/log/kern.log
    fi
elif [ $1 -eq 3 ]; then
    insmod CharDriver.ko
    if [ $? -eq 0 ]; then
        echo "Module loaded"
    else
        if [ $2 -eq 1 ]; then
            tail /var/log/kern.log
        fi
        echo "FAIL"
        exit
    fi

    rmmod CharDriver.ko
    if [ $? -eq 0 ]; then
        echo "Module unloaded"
    else
        if [ $2 -eq 1 ]; then
            tail /var/log/kern.log
        fi
        echo "FAIL"
        exit
    fi
    if [ $2 -eq 1 ]; then
        tail /var/log/kern.log
    fi
else
    echo "Invalid mode!"
    echo "Provide execution mode"
    echo "Parameter 1:"
    echo "  > 0 : dmesg"
    echo "  > 1 : Load module"
    echo "  > 2 : Unload module"
    echo "  > 3 : Load and unload moule"
    echo "Parameter 2:"
    echo "  > 0 : desable tail"
    echo "  > 1 : enable tail"
fi
