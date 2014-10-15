delimiter ;


drop procedure  if exists ClassesRetrieveAll;

delimiter //

create procedure ClassesRetrieveAll()
begin
  select   c.ClassId,
           c.ClassName,
           c.ParentId,
           (select c1.ClassName  from Classes c1  where c1.ClassId = c.ParentId)  as  ParentName,
           c.Description

    from Classes c
    order by c.ClassName;
end
//

delimiter ;






/**********************************************************************************************************************/
drop procedure    if exists ClassesRetrieveByName;

delimiter //

create procedure ClassesRetrieveByName(in  _ClassName  varChar(64))
BEGIN
  select   c.ClassId,
           c.ClassName,
           c.ParentId,
           (select c1.ClassName  from Classes c1  where c1.ClassId = c.ParentId)  as  ParentName,
           c.Description
    from Classes c
    where c.ClassName = _ClassName;
END
//

delimiter ;






/**********************************************************************************************************************/
drop procedure    if exists ImageClassInsert;
drop procedure    if exists MLClassInsert;

delimiter //

CREATE PROCEDURE MLClassInsert(in  _ClassName    varChar(64),
                               in  _ParentName   varChar(64),
                               in  _Description  varChar(255)
                              )
BEGIN
  insert into Classes(ClassName, ParentId, Description)
         values(_ClassName,
                (select  c1.ClassId  from Classes c1  where  c1.ClassName = _ParentName),
                _Description
               );
END
//

delimiter ;






/**********************************************************************************************************************/
drop procedure   if exists  ImageClassDelete;
drop procedure   if exists  MLClassDelete;

delimiter //

create procedure MLClassDelete(in  _className    varChar(64))
begin
  set @classId  = (select c.ClassId  from Classes c where  c.ClassName = _className);
  set @parentId = (select c.ParentId from Classes c where  c.ClassName = _className);
  
  update  Classes
       set ParentId = @parentId
       where  ParentId = @classId;
       
  update  Classes
       set ParentId = @parentId
       where  ParentId = @classId;
   

  delete from Classes  where  ClassId = @classId;

  /* select  @classId, @parentId; */

end
//

delimiter ;






/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassUpdate;
drop procedure   if exists  MLClassUpdate;

delimiter //

CREATE PROCEDURE  MLClassUpdate(in  OldClassName    varChar(64),
                                in  NewClassName    varChar(64),
                                in  NewParentName   varChar(64),
                                in  NewDescription  varChar(255)
                               )
BEGIN
  set  @newParentId = (select c1.classId from Classes c1 where c1.ClassName = NewParentName);


  update  Classes c
    set  c.ClassName    = NewClassName,
         c.ParentId     = @newParentId,
         c.Description  = NewDescription
     where  c.ClassName = OldClassName;
END
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassInsertReturn;
drop procedure   if exists  MLClassInsertReturn;

delimiter //

create procedure MLClassInsertReturn (in  _name        varChar(64),
                                      in  _parentName  varChar(64)
                                     )
begin
  declare   _classId           int  default 0;
  declare   _parentId          int  default 0;

  set  _classId  = (select c1.ClassId from  Classes c1  where  c1.ClassName = _name);
  set  _parentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _parentName);

  if  (_classId is null)  then
    call MLClassInsert (_name, _parentName, "Added by 'MLClassInsertReturn'");
  end if;


  select * from  Classes c3  where  c3.ClassName = _name;
end
//
delimiter ;







/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassMerge;
drop procedure   if exists  MLClassMerge;

delimiter //

create procedure  MLClassMerge (in  _srcClassName     varChar(64),
                                in  _destClassName    varChar(64)
                               )
begin
  set  @oldParentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _srcClassName);
  set  @newParentId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _destClassName);


  update  Classes c
     set c.ParentId = @newParentId
     where  c.ParentId = @oldParentId;
     
  delete  from  Classes
     where  ClassId = @oldParentId;

end
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassMergeComprehensive;
drop procedure   if exists  MLClassMergeComprehensive;

delimiter //

create procedure  MLClassMergeComprehensive (in  _srcClassName     varChar(64),
                                             in  _destClassName    varChar(64)
                                            )
WholeProcedure:  begin
  set  @srcClassId  = (select c1.ClassId from  Classes c1  where  c1.ClassName = _srcClassName);
  set  @destClassId = (select c1.ClassId from  Classes c1  where  c1.ClassName = _destClassName);

  if  @srcClassId  is null  then
    select  concat(_srcClassName, " Does not  exist");
    leave WholeProcedure;
  end if;

  if  @destClassId  is null  then
    select  concat(_destClassName, " Does not  exist");
    leave WholeProcedure;
  end if;
  
  select  "Starting ", _srcClassName, _destClassName;

  update  Classes c  set c.ParentId = @destClassId    where  c.ParentId = @srcClassId;
  
  set  @totalClass1IdUpdated = 0;
  LoopClass1Id: loop
    update Images i  set  Class1Id = @destClassId  where  Class1Id = @srcClassId  limit 100;
    set @rc = ROW_COUNT();
    set @totalClass1IdUpdated =  @totalClass1IdUpdated + @rc;

    if  @rc < 100  then
       leave LoopClass1Id;
    end if;
  end loop  LoopClass1Id;
 
  set  @totalClass2IdUpdated = 0;
  LoopClass2Id: loop
    update Images i  set  Class2Id = @destClassId  where  Class2Id = @srcClassId  limit 100;
    set @rc = ROW_COUNT();
    set @totalClass2IdUpdated =  @totalClass2IdUpdated + @rc;
    commit;
    if  @rc < 100  then
       leave LoopClass2Id;
    end if;
  end loop  LoopClass2Id;


  set  @totalClassValidatedIdUpdated = 0;
  LoopClassValidatedId: loop
    update Images i  set  ClassValidatedId = @destClassId  where  ClassValidatedId = @srcClassId  limit 100;
    set @rc = ROW_COUNT();
    set @totalClassValidatedIdUpdated =  @totalClassValidatedIdUpdated + @rc;
    commit;
    if  @rc < 100  then
       leave LoopClassValidatedId;
    end if;
  end loop  LoopClassValidatedId;
     
  delete  from  Classes  where  ClassId = @srcClassId;

  select  _srcClassName, _destClassName, @totalClass1IdUpdated, @totalClass2IdUpdated, @totalClassValidatedIdUpdated;
end
//
delimiter ;




/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  ImageClassDeleteComprehensive;
drop procedure   if exists  MLClassDeleteComprehensive;

delimiter //

create procedure MLClassDeleteComprehensive (in  _className     varChar(64))
begin
  set  @srcClassId  = (select c1.ClassId  from  Classes c1  where  c1.ClassName = _className);
  set  @destClassId = (select c1.ParentId from  Classes c1  where  c1.ClassName = _className);

  update  Classes c  set c.ParentId = @destClassId    where  c.ParentId = @srcClassId;

  update Images i  set  Class1Id         = @destClassId    where  Class1Id         = @srcClassId;
  update Images i  set  Class2Id         = @destClassId    where  Class2Id         = @srcClassId;
  update Images i  set  ClassValidatedId = @destClassId    where  ClassValidatedId = @srcClassId;
     
  delete  from  Classes  where  ClassId = @srcClassId;
end
//
delimiter ;












/**********************************************************************************************************************/
/**********************************************************************************************************************/
drop procedure   if exists  RenameWholeBunch;

delimiter //

create procedure  RenameWholeBunch ()
begin
  call  MLClassMergeComprehensive ("Crustacean_copepod_corycaeus",              "crustacean_copepod_poecilostomatoid");
  call  MLClassMergeComprehensive ("crustacean_copepod_eyes",                   "crustacean_copepod_poecilostomatoid");
  call  MLClassMergeComprehensive ("crustacean_copepod_oncaea",                 "crustacean_copepod_poecilostomatoid");
  call  MLClassMergeComprehensive ("detritus_floc",                             "detritus_snow");
  call  MLClassMergeComprehensive ("detritus_marinesnow",                       "detritus_snow");
  call  MLClassMergeComprehensive ("detritus_oil",                              "detritus_snow");
  call  MLClassMergeComprehensive ("detritus_oildroplet",                       "unknown_roundobjects");
  call  MLClassMergeComprehensive ("elongate_strands",                          "phytoplankton_strands");
  call  MLClassMergeComprehensive ("elongate_string",                           "phytoplankton_strands");
  call  MLClassMergeComprehensive ("Gelatinous_cnidaria",                       "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_aequorea",          "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_blunt",             "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_dark_squat",        "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_liriope",           "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_long",              "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_narcomedusae",      "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_neoturris",         "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_polyorchis",        "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_sarsia",            "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_scyphazoa_pelagia", "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_solmaris",          "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_solmundella",       "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_hydromedusae_velella",           "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_medusae_solmundella",	           "gelatinous_hydromedusae_other");
  call  MLClassMergeComprehensive ("gelatinous_salp",                           "gelatinous_tunicate_salp");
  call  MLClassMergeComprehensive ("gelatinous_siphonophore_A",                 "gelatinous_siphonophore");
  call  MLClassMergeComprehensive ("gelatinous_siphonophore_calycophoran",      "gelatinous_siphonophore");
  call  MLClassMergeComprehensive ("gelatinous_siphonophore_hippodious",        "gelatinous_siphonophore");
  call  MLClassMergeComprehensive ("gelatinous_siphonophore_prayid",            "gelatinous_siphonophore");
  call  MLClassMergeComprehensive ("gelatinous_siphonophore_sphaeronectes",     "gelatinous_siphonophore");
  call  MLClassMergeComprehensive ("Gelatinous_tunicate_doliolid-tail",         "gelatinous_tunicate_doliolid");
  call  MLClassMergeComprehensive ("gelatinous_tunicate_salp_chain",            "gelatinous_tunicate_salp");
  call  MLClassMergeComprehensive ("larvacean_bare",                            "larvacean");
  call  MLClassMergeComprehensive ("larvacean_large",                           "larvacean");
  call  MLClassMergeComprehensive ("larvacean_tail",                            "larvacean");
  call  MLClassMergeComprehensive ("larvacean_tectillaria",                     "larvacean");
  call  MLClassMergeComprehensive ("larvae_holothurian",                        "echinoderm_bipinnaria");
  call  MLClassMergeComprehensive ("mollusc_heteropod_atlanta",                 "mollusc_atlanta");
  call  MLClassMergeComprehensive ("mollusc_pteropod",                          "mollusc_pteropod_assorted");
  call  MLClassMergeComprehensive ("mollusc_pteropod_clio",                     "mollusc_pteropod_conical");
  call  MLClassMergeComprehensive ("mollusc_pteropod_corolla",                  "mollusc_pteropod_pseudothecosomata");
  call  MLClassMergeComprehensive ("mollusc_pteropod_creseis",                  "mollusc_pteropod_conical");
  call  MLClassMergeComprehensive ("mollusc_pteropod_creseis_a",                "mollusc_pteropod_conical");
  call  MLClassMergeComprehensive ("mollusc_pteropod_creseis_b",                "mollusc_pteropod_conical");
  call  MLClassMergeComprehensive ("mollusc_pteropod_desmopteris",              "mollusc_pteropod_pseudothecosomata");
  call  MLClassMergeComprehensive ("noctiluca",                                 "protist_noctiluca");
  call  MLClassMergeComprehensive ("noise_BigBlobs",                            "noise_lines");
  call  MLClassMergeComprehensive ("noise_plus",                                "noise_lines");
  call  MLClassMergeComprehensive ("noise_streaks",                             "noise_lines");
  call  MLClassMergeComprehensive ("other_fish",                                "fish");
  call  MLClassMergeComprehensive ("other_polychaete",                          "polychaete");
  call  MLClassMergeComprehensive ("Other_pteropod",                            "mollusc_pteropod");
  call  MLClassMergeComprehensive ("phytoplankton_centric",                     "phytoplankton_diatomcentric");
  call  MLClassMergeComprehensive ("phytoplankton_chaetoceros",                 "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_chain",                       "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_diatom",                      "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_diatom_centric",              "phytoplankton_diatomcentric");
  call  MLClassMergeComprehensive ("phytoplankton_diatom_chaetoceros",          "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_diatom_chain",                "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_diatom_coil",                 "phytoplankton_diatomcoil");
  call  MLClassMergeComprehensive ("phytoplankton_noctiluca",                   "protist_noctiluca");
  call  MLClassMergeComprehensive ("phytoplankton_pennate",                     "phytoplankton_strands");
  call  MLClassMergeComprehensive ("phytoplankton_strings",                     "phytoplankton_strands");
  call  MLClassMergeComprehensive ("protist_bilobed",                           "protist_noctiluca");
  call  MLClassMergeComprehensive ("protist_cluster",                           "detritus_protist");
  call  MLClassMergeComprehensive ("protist_D",                                 "protist");
  call  MLClassMergeComprehensive ("protist_darkcenter",                        "protist");
  call  MLClassMergeComprehensive ("protist_diffuse",                           "protist_unknown");
  call  MLClassMergeComprehensive ("protist_E",                                 "protist");
  call  MLClassMergeComprehensive ("protist_F",                                 "protist");
  call  MLClassMergeComprehensive ("protist_finned",                            "protist_radio");
  call  MLClassMergeComprehensive ("protist_foram",                             "protist");
  call  MLClassMergeComprehensive ("protist_G",                                 "protist");
  call  MLClassMergeComprehensive ("protist_H",                                 "protist");
  call  MLClassMergeComprehensive ("protist_I",                                 "protist");
  call  MLClassMergeComprehensive ("protist_J",                                 "protist");
  call  MLClassMergeComprehensive ("protist_K",                                 "protist");
  call  MLClassMergeComprehensive ("protist_lobed",                             "protist");
  call  MLClassMergeComprehensive ("protist_lopsided",                          "protist_radio");
  call  MLClassMergeComprehensive ("protist_multiple",                          "detritus_protist");
  call  MLClassMergeComprehensive ("protist_other",                             "protist");
  call  MLClassMergeComprehensive ("protist_phage",                             "protist");
  call  MLClassMergeComprehensive ("protist_phi",                               "protist");
  call  MLClassMergeComprehensive ("protist_point",                             "protist");
  call  MLClassMergeComprehensive ("protist_pseudopodia",                       "protist");
  call  MLClassMergeComprehensive ("protist_rad",                               "protist");
  call  MLClassMergeComprehensive ("protist_rad1",                              "protist");
  call  MLClassMergeComprehensive ("protist_rad2",                              "protist");
  call  MLClassMergeComprehensive ("protist_radcolony",                         "protist");
  call  MLClassMergeComprehensive ("protist_radgel",                            "protist");
  call  MLClassMergeComprehensive ("protist_radiolarian",                       "protist_acantharia");
  call  MLClassMergeComprehensive ("protist_radknob",                           "protist_acantharia");
  call  MLClassMergeComprehensive ("protist_spiny",                             "protist_acantharia");
  call  MLClassMergeComprehensive ("protist_thallasicola",                      "protist_thalassicola");
  call  MLClassMergeComprehensive ("radiolarian_colony",                        "protist_spumellaria");
  call  MLClassMergeComprehensive ("radiolarian_ribboncolony",                  "protist_spumellaria");
  call  MLClassMergeComprehensive ("radiolarian_roundcolony",                   "protist_spumellaria");
  call  MLClassMergeComprehensive ("trichodesmium",                             "phytoplankton_trichodesmium");
  call  MLClassMergeComprehensive ("trichodesmium_colonies",                    "phytoplankton_trichodesmium");
  call  MLClassMergeComprehensive ("Trichodesmium_elongate",                    "phytoplankton_trichodesmium");
  call  MLClassMergeComprehensive ("UnKnown",                                   "other");
  call  MLClassMergeComprehensive ("unknown_A",                                 "other");
  call  MLClassMergeComprehensive ("unknown_B",                                 "other");
  call  MLClassMergeComprehensive ("UnKnown_Kurt",                              "other");
  
  
  call  MLClassDeleteComprehensive ("elongate");
  call  MLClassDeleteComprehensive ("Fish_Sticks");
  call  MLClassDeleteComprehensive ("other_rare");
  call  MLClassDeleteComprehensive ("other_streaks");
  call  MLClassDeleteComprehensive ("other_teardrop");
  call  MLClassDeleteComprehensive ("out_of_focus");
end
//
delimiter ;

