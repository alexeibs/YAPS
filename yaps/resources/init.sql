create table passwords(id integer primary key autoincrement, name varchar(1024) not null, password varchar(1024) null, created_at timestamp not null default 0);
create index name_index on passwords(name);
INSERT INTO passwords(name, password) VALUES("test1", "pass1");
INSERT INTO passwords(name, password) VALUES("test2", "pass2");
INSERT INTO passwords(name, password) VALUES("test3", "pass3");
INSERT INTO passwords(name, password) VALUES("test4", "pass4");