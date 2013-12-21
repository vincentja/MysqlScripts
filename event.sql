Select @@event_scheduler; 
SET GLOBAL event_scheduler = ON; 

vi /etc/mysql/my.cnf
[mysqld]
event_scheduler=1  --需校验从库调度事件

/etc/init.d./mysql restart

DROP EVENT IF EXISTS  event_clear_hashs;
CREATE EVENT event_clear_hashs
    ON SCHEDULE
      EVERY 360 SECOND
    DO
      delete from images.hashs;
      
 show events\G
 -- 206 53940 条记录，15:53
 ----------------------------------------------
-- 1) 临时关闭事件
ALTER EVENT e_test DISABLE;
-- 2) 开启事件
ALTER EVENT e_test ENABLE; 
      
-- http://blog.csdn.net/crazylaa/article/details/5368819
