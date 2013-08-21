create table passwords(name varchar(1024) primary key null, password varchar(1024) null, changed_at timestamp not null default 0);
create index name_index on passwords(name);