

-- Remove conflicting tables
DROP TABLE IF EXISTS at_war CASCADE;
DROP TABLE IF EXISTS economy CASCADE;
DROP TABLE IF EXISTS geography CASCADE;
DROP TABLE IF EXISTS millitarism CASCADE;
DROP TABLE IF EXISTS nation CASCADE;
DROP TABLE IF EXISTS politics CASCADE;
DROP TABLE IF EXISTS resources CASCADE;
DROP TABLE IF EXISTS society CASCADE;
-- End of removing

CREATE TABLE at_war (
    type_of_war VARCHAR(256) NOT NULL,
    id INTEGER NOT NULL
);
ALTER TABLE at_war ADD CONSTRAINT pk_at_war PRIMARY KEY (type_of_war);

CREATE TABLE economy (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    gdp DOUBLE PRECISION NOT NULL,
    stability DOUBLE PRECISION NOT NULL,
    curruption_rate DOUBLE PRECISION NOT NULL
);
ALTER TABLE economy ADD CONSTRAINT pk_economy PRIMARY KEY (id);
ALTER TABLE economy ADD CONSTRAINT u_fk_economy_nation UNIQUE (nation_id);

CREATE TABLE geography (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    aver_temp DOUBLE PRECISION NOT NULL,
    size DOUBLE PRECISION NOT NULL
);
ALTER TABLE geography ADD CONSTRAINT pk_geography PRIMARY KEY (id);
ALTER TABLE geography ADD CONSTRAINT u_fk_geography_nation UNIQUE (nation_id);

CREATE TABLE millitarism (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    conscript DOUBLE PRECISION NOT NULL,
    tank_num INTEGER NOT NULL,
    troop_num INTEGER NOT NULL,
    rocket_num INTEGER NOT NULL,
    bomb_num INTEGER NOT NULL,
    mil_factories INTEGER NOT NULL,
    nuke INTEGER NOT NULL,
    rifle_num INTEGER NOT NULL,
    tactic VARCHAR(256) NOT NULL
);
ALTER TABLE millitarism ADD CONSTRAINT pk_millitarism PRIMARY KEY (id);
ALTER TABLE millitarism ADD CONSTRAINT u_fk_millitarism_nation UNIQUE (nation_id);

CREATE TABLE nation (
    id SERIAL NOT NULL,
    country_name VARCHAR(256) NOT NULL,
    capital_name VARCHAR(256)
);
ALTER TABLE nation ADD CONSTRAINT pk_nation PRIMARY KEY (id);
ALTER TABLE nation ADD CONSTRAINT uc_nation_country_name UNIQUE (country_name);

CREATE TABLE politics (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    ideology VARCHAR(256) NOT NULL,
    fraction VARCHAR(256) NOT NULL,
    gover_supp DOUBLE PRECISION NOT NULL
);
ALTER TABLE politics ADD CONSTRAINT pk_politics PRIMARY KEY (id);
ALTER TABLE politics ADD CONSTRAINT u_fk_politics_nation UNIQUE (nation_id);

CREATE TABLE resources (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    oil INTEGER NOT NULL,
    gas INTEGER NOT NULL,
    diamond INTEGER NOT NULL,
    gold INTEGER NOT NULL,
    iron INTEGER NOT NULL,
    coal INTEGER NOT NULL
);
ALTER TABLE resources ADD CONSTRAINT pk_resources PRIMARY KEY (id);
ALTER TABLE resources ADD CONSTRAINT u_fk_resources_nation UNIQUE (nation_id);

CREATE TABLE society (
    id SERIAL NOT NULL,
    nation_id INTEGER NOT NULL,
    population INTEGER NOT NULL,
    pop_growth DOUBLE PRECISION NOT NULL,
    pop_death DOUBLE PRECISION NOT NULL,
    lifespan DOUBLE PRECISION NOT NULL,
    tax DOUBLE PRECISION NOT NULL,
    happiness DOUBLE PRECISION NOT NULL
);
ALTER TABLE society ADD CONSTRAINT pk_society PRIMARY KEY (id);
ALTER TABLE society ADD CONSTRAINT u_fk_society_nation UNIQUE (nation_id);

ALTER TABLE at_war ADD CONSTRAINT fk_at_war_nation FOREIGN KEY (id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE economy ADD CONSTRAINT fk_economy_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE geography ADD CONSTRAINT fk_geography_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE millitarism ADD CONSTRAINT fk_millitarism_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE politics ADD CONSTRAINT fk_politics_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE resources ADD CONSTRAINT fk_resources_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;

ALTER TABLE society ADD CONSTRAINT fk_society_nation FOREIGN KEY (nation_id) REFERENCES nation (id) ON DELETE CASCADE;







