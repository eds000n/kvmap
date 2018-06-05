#!/bin/bash

source .config
host="http://$HOST:$PORT"
putCmd="curl -X POST $host/value?"
deleteCmd="curl -X DELETE $host/value?"
getCmd="curl -X GET $host/value?"



DIALOG_CANCEL=1
DIALOG_ESC=255
HEIGHT=0
WIDTH=0

display_result() {
  dialog --title "$1" \
    --no-collapse \
    --msgbox "$result" 0 0
}

while true; do
  exec 3>&1
  selection=$(dialog \
    --backtitle "KVMap client" \
    --title "Menu" \
    --clear \
    --cancel-label "Exit" \
    --menu "Please select:" $HEIGHT $WIDTH 4 \
    "1" "Put key,value" \
    "2" "Delete by key" \
    "3" "Get by key" \
    2>&1 1>&3)
  exit_status=$?
  exec 3>&-
  case $exit_status in
    $DIALOG_CANCEL)
      clear
      echo "Program terminated."
      exit
      ;;
    $DIALOG_ESC)
      clear
      echo "Program aborted." >&2
      exit 1
      ;;
  esac
  case $selection in
    0 )
      clear
      echo "Program terminated."
      ;;
    1 )
      #echo "Insert key"
      #read key
      #echo "Insert value"
      #read value
      key=$(dialog --title "PUT" --inputbox "Insert the key" 0 0 3>&1 1>&2 2>&3 3>&-)
      value=$(dialog --title "PUT" --inputbox "Insert the value" 0 0 3>&1 1>&2 2>&3 3>&-)
      result=$(curl -X POST "$host/value?key=$key&value=$value")
      display_result "Put (123,adf)"
      ;;
    2 )
      #echo "Insert key"
      #read key
      key=$(dialog --title "DELETE" --inputbox "Insert the key" 0 0 3>&1 1>&2 2>&3 3>&-)
      result=$(curl -X DELETE "$host/value?key=$key")
      display_result "Delete by key"
      ;;
    3 )
      #echo "Insert key"
      #read key
      key=$(dialog --title "GET" --inputbox "Insert the key" 0 0 3>&1 1>&2 2>&3 3>&-)
      result=$(curl -X GET "$host/value?key=$key")
      display_result "Get by key"
      #if [[ $(id -u) -eq 0 ]]; then
      #  result=$(du -sh /home/* 2> /dev/null)
      #  display_result "Home Space Utilization (All Users)"
      #else
      #  result=$(du -sh $HOME 2> /dev/null)
      #  display_result "Home Space Utilization ($USER)"
      #fi
      ;;
  esac
done
