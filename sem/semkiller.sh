set IPCS_S = ipcs -s | grep nxh177 | cut -f2 -d " "
for id in $IPCS_S; do
   ipcrm -s $id;
done

