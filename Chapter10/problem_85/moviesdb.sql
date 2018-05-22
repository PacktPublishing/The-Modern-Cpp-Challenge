create table movies(
   title text not null,
   year integer not null,
   length integer not null);

create table persons(name text not null);

create table directors(
   movieid integer not null,
   personid integer not null);

create table writers(
   movieid integer not null,
   personid integer not null);

create table casting(
   movieid integer not null,
   personid integer not null,
   role text not null);   

insert into movies values ('The Matrix', 1999, 196);
insert into movies values ('Forrest Gump', 1994, 202);

insert into persons values('Keanu Reeves');
insert into persons values('Laurence Fishburne');
insert into persons values('Carrie-Anne Moss');
insert into persons values('Hugo Weaving');
insert into persons values('Lana Wachowski');
insert into persons values('Lilly Wachowski');
insert into persons values('Tom Hanks');
insert into persons values('Sally Field');
insert into persons values('Robin Wright');
insert into persons values('Mykelti Williamson');
insert into persons values('Robert Zemeckis');
insert into persons values('Winston Groom');
insert into persons values('Eric Roth');

insert into directors values(1, 5);
insert into directors values(1, 6);
insert into directors values(2, 11);

insert into writers values(1, 5);
insert into writers values(1, 6);
insert into writers values(2, 12);
insert into writers values(2, 13);

insert into casting values(1, 1, 'Neo');
insert into casting values(1, 2, 'Morpheus');
insert into casting values(1, 3, 'Trinity');
insert into casting values(1, 4, 'Agent Smith');
insert into casting values(2, 7, 'Forrest Gump');
insert into casting values(2, 8, 'Mrs. Gump');
insert into casting values(2, 9, 'Jenny Curran');
insert into casting values(2, 10, 'Bubba Blue');