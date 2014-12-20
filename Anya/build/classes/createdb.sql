CREATE TABLE BUILDINGS (BUILDING_ID VARCHAR2(40) primary key, BUILDING_NAME VARCHAR(40), VERTICES NUMBER(5), COORDINATES SDO_GEOMETRY);
CREATE TABLE ASYSTEM (AS_ID VARCHAR2(7) primary key,POSITION SDO_GEOMETRY,RADIUS NUMBER(5),CIRCLE SDO_GEOMETRY);
CREATE TABLE STUDENTS (PERSON_ID VARCHAR2(5) primary key, SCOORDINATES SDO_GEOMETRY);
--------------------------------metadata insert statements---------------------------------------
----asystem position------
INSERT INTO user_sdo_geom_metadata
    (TABLE_NAME,
     COLUMN_NAME,
     DIMINFO,
     SRID)
  VALUES (
  'asystem',
  'position',
  SDO_DIM_ARRAY(   -- 20X20 grid
    SDO_DIM_ELEMENT('X', 0, 20, 0.005),
    SDO_DIM_ELEMENT('Y', 0, 20, 0.005)
     ),
  NULL   -- SRID
);
----asystem circle------
INSERT INTO user_sdo_geom_metadata
    (TABLE_NAME,
     COLUMN_NAME,
     DIMINFO,
     SRID)
  VALUES (
  'asystem',
  'circle',
  SDO_DIM_ARRAY(   -- 20X20 grid
    SDO_DIM_ELEMENT('X', 0, 20, 0.005),
    SDO_DIM_ELEMENT('Y', 0, 20, 0.005)
     ),
  NULL   -- SRID
);
----students------
INSERT INTO user_sdo_geom_metadata
    (TABLE_NAME,
     COLUMN_NAME,
     DIMINFO,
     SRID)
  VALUES (
  'students',
  'scoordinates',
  SDO_DIM_ARRAY(   -- 20X20 grid
    SDO_DIM_ELEMENT('X', 0, 20, 0.005),
    SDO_DIM_ELEMENT('Y', 0, 20, 0.005)
     ),
  NULL   -- SRID
);
----buildings------


INSERT INTO user_sdo_geom_metadata
    (TABLE_NAME,
     COLUMN_NAME,
     DIMINFO,
     SRID)
  VALUES (
  'buildings',
  'coordinates',
  SDO_DIM_ARRAY(   -- 20X20 grid
    SDO_DIM_ELEMENT('X', 0, 20, 0.005),
    SDO_DIM_ELEMENT('Y', 0, 20, 0.005)
     ),
  NULL   -- SRID
);
----------------------------------------------------------------------------------------------------

CREATE INDEX student_index ON students(scoordinates)
INDEXTYPE IS MDSYS.SPATIAL_INDEX; 

CREATE INDEX asystem_index_pos ON asystem(position)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

CREATE INDEX asystem_index_cir ON asystem(circle)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;


CREATE INDEX building_index ON buildings(coordinates)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;
