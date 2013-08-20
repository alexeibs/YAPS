create table passwords(name varchar(1024) primary key null, password varchar(1024) null, changed_at timestamp not null default 0);
create index name_index on passwords(name);
INSERT INTO passwords(name, password) VALUES("test1", "pass1");
INSERT INTO passwords(name, password) VALUES("test2", "pass2");
INSERT INTO passwords(name, password) VALUES("test3", "pass3");
INSERT INTO passwords(name, password) VALUES("test4", "pass4");