#ifndef  _ATTRIBUTE_
#define  _ATTRIBUTE_

/**
 *@class  MLL::Attribute
 *@author  Kurt Kramer
 *@brief describes a single Feature, Type and possible values.
 *@details Used to support 'FileDesc', 'FeatureVector', and 'ImageFeatures' classes.  FileDesc 
 *         will maintain a list of 'Attribute' objects to describe each separate field in a given
 *         FeatureFile.  A given Feature can be one of several types, Numeric, Nominal, Ordinal, 
 *         and Symbolic.
 *
 *@code
 * Numeric  - Any floating point value.
 * Nominal  - Feature must be one of a specified possible values.  The Attribute class will
 *            maintain a list of possible values.  This will be saved as a integer value from
 *            in the FeatureVector object.  When training a classifier it would be best to
 *            bit encode these features.  See the FeatureEncoder class.
 * Ordinal  - Similar to Nominal except there is a definite ordering of value.
 * Symbolic - Similar to Nominal except you do not know all the possible values this attribute
 *            can take on.
 *@endcode
 *@see  MLL::FeatureEncoder, MLL::FeatureVector, MLL::FeatureFileIO
*/
#include  <map>

#include  "Str.h"


namespace MLL 
{
  typedef  enum 
  {
    IgnoreAttribute,
    NumericAttribute,
    NominalAttribute,
    OrdinalAttribute,
    SymbolicAttribute,   /**< Same as NominalAttribute, except the names file does not
                          * list all possible values.  They have to be determined from
                          * the data file.
                          */
    NULLAttribute
  } 
  AttributeType;

  typedef  std::vector<AttributeType>  AttributeTypeVector;

  typedef  AttributeTypeVector*        AttributeTypeVectorPtr;

  class  Attribute
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    Attribute (const KKStr&   _name,
               AttributeType  _type,
               int32          _fieldNum
              );

    Attribute (const Attribute&  a);

    ~Attribute ();

    /**
      *@brief  Adds a allowable Nominal value to the Nominal or Symbolic field that this attribute represents.
      *@details To only be used by instances of 'Attribute' that represent Nominal or Symbolic type attributes.  If the
      *         Attribute type is not s 'Nominal' or 'Symbolic' then this method will throw am exception
      *@param[in] nominalValue A possible value that this instance of 'Attribute' could represent.
      *@param[out] alreadyExists Indicates if this instance of 'Attribute' already contains a nominal value called 'nominalValue'.
      */
    void           AddANominalValue (const KKStr&  nominalValue,
                                     bool&         alreadyExists
                                    );

    /**
     *@brief Returns back the cardinality of the attribute; the number of possible values it can take.
     *@details Only  attributes with type NominalAttribute or SymbolicAttribute have a fixed number
     *         of possible values all others will return 999999999.
     */
    int32          Cardinality ();

    int32          FieldNum ()  const  {return  fieldNum;}

    int32          GetNominalCode  (const KKStr&  nominalValue)  const;  // -1 means not found.

    /**
     *@brief  Returns the nominal value for the given ordinal value.
     *@details For example: you could have a Attribute called DayOfTheWeek that would be type 'NominalAttribute'
     *  where its possible values  are "Sun", "Mon", "Tue", "Wed", "Thur", "Fri", and "Sat".   In this case a call
     *  to this method where 'code' == 3 would return "Wed".
     */
    const  
    KKStr&         GetNominalValue (int32 code) const;

    int32          MemoryConsumedEstimated ()  const;

    const
    KKStr&         Name () const {return name;}

    const
    KKStr&         NameUpper () const {return nameUpper;}
  
    AttributeType  Type () const {return  type;}

    KKStr          TypeStr () const;

    Attribute&     operator= (const Attribute&  right);

    bool  operator== (const Attribute&  rightSide) const;
    bool  operator!= (const Attribute&  rightSide) const;

  private:
    void    ValidateNominalType (const KKStr&  funcName)  const;

    int32              fieldNum;
    KKStr              name;
    KKStr              nameUpper;
    KKStrListIndexed*  nominalValuesUpper;
    AttributeType      type;
  };  /* Attribute */

  typedef  Attribute*  AttributePtr;


  class  AttributeList: public KKQueue<Attribute>
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    AttributeList (bool  owner,
                   int32 growthRate
                  );

    ~AttributeList ();

    AttributeTypeVectorPtr  CreateAttributeTypeVector ()  const;

    const
    AttributePtr  LookUpByName (const KKStr&  attributeName)  const;

    int32   MemoryConsumedEstimated ()  const;

    void  PushOnBack   (AttributePtr  attribute);

    void  PushOnFront  (AttributePtr  attribute);

    /** @brief  Determines if two different attribute lists are the same by comparing each respective attribute, name and type.   */
    bool  operator== (const AttributeList&  right)  const;

    /**  @brief  Determines if two different attribute lists are different by comparing each respective attribute, name and type. */
    bool  operator!= (const AttributeList&  right)  const;

  private:
    void  AddToNameIndex (AttributePtr  attribute);
  
    std::map<KKStr, AttributePtr>  nameIndex;
  };  /* AttributeList */

  KKStr  AttributeTypeToStr (AttributeType  type);

}  /* namespace MLL */

#endif
