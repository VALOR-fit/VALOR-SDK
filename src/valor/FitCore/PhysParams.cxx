#include "valor/FitCore/PhysParams.h"

#include <vector>

// Root
#include <TSystem.h>

// valor
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"

namespace valor
{

PhysParams* PhysParams::_instance = nullptr;

using std::string;
using std::vector;
using util::UtlFile;

PhysParams::PhysParams():
   _defaults{new Param("default", Undefined)},
   _nextId{0}
{

}

PhysParams::~PhysParams()
{
   // Delete the individual parameters from the map
   for(auto param : _parameters)
   {
      delete param.second;
   }
   _parameters.clear();
   delete _defaults; _defaults = nullptr;
   delete _instance; _instance = nullptr;
}

PhysParams* PhysParams::Instance()
{
   if(not(_instance))
   {
      _instance = new PhysParams();
   }

   return _instance;
}

void PhysParams::Read(const string& filename)
{
   string fullFilename = UtlFile::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("PhysParams", pERROR) << "Config file not found: " << filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   TXMLEngine* xml = new TXMLEngine();
   XMLDocPointer_t xmlDoc = xml->ParseFile(fullFilename.c_str());
   if(xmlDoc == 0)
   {
      LOG("PhysParams", pERROR) << "Cannot parse XML file [" << fullFilename <<
         "]";
      delete xml;
      util::Die(1);
   }

   XMLNodePointer_t rootNode = xml->DocGetRootElement(xmlDoc);
   XMLNodePointer_t defaultsNode = util::FindNode(xml, rootNode, "defaults");

   // Get oscillation parameter node
   ProcessDefaults(xml, defaultsNode);

   // Process parameter values
   vector<XMLNodePointer_t> params = util::FindNodes(xml, rootNode,
         "parameter");
   for(auto node : params)
   {
      ProcessParameter(xml, node);
   }

   delete xml;
}

void PhysParams::Select(const string& filename)
{
   string fullFilename = UtlFile::GetConfigurationFilename(filename);

   // Check that the config file exists.
   if(fullFilename.empty())
   {  // File not found
      LOG("PhysParams", pERROR) << "Config file not found: " << filename;
      util::Die(1);
   }

   // Parse the XML configuration file
   TXMLEngine* xml = new TXMLEngine();
   XMLDocPointer_t xmlDoc = xml->ParseFile(fullFilename.c_str());
   if(xmlDoc == 0)
   {
      LOG("PhysParams", pERROR) << "Cannot parse XML file [" << fullFilename <<
         "]";
      delete xml;
      util::Die(1);
   }

   XMLNodePointer_t rootNode = xml->DocGetRootElement(xmlDoc);

   // Get oscillation parameter node
   ProcessParameters(util::XML2String(xml, rootNode, "osc_params/param_list"));

   // Get systematic parameters
   vector<XMLNodePointer_t> sysParams = util::FindNodes(xml, rootNode,
         "rule");
   for(auto node : sysParams)
   {
      ProcessParameters(util::XML2String(xml, node, "param_list"));
   }

   delete xml;
}

double PhysParams::Bestfit(const std::string& param) const
{
   StrictCheck(param, "best fit"); 
   return _parameters.at(param)->_bestfit;
}

double PhysParams::Initial(const std::string& param) const
{
   StrictCheck(param, "initial value");
   return _parameters.at(param)->_initial;
}

double PhysParams::Minimum(const std::string& param) const
{
   StrictCheck(param, "minimum value"); 
   return _parameters.at(param)->_minimum;
}

double PhysParams::Maximum(const std::string& param) const
{
   StrictCheck(param, "maximum value");
   return _parameters.at(param)->_maximum;
}

double PhysParams::StepSize(const std::string& param) const
{
   StrictCheck(param, "step size");
   return _parameters.at(param)->_stepsize;
}

double PhysParams::Scaling(const std::string& param) const
{
   StrictCheck(param, "scaling");
   return _parameters.at(param)->_scaling;
}

bool PhysParams::IsMarginalised(const std::string& param) const
{
   StrictCheck(param, "marginalisation status");
   return _parameters.at(param)->_isMarginalised;
}

double PhysParams::GaussianPriorMean(const std::string& param) const
{
   StrictCheck(param, "Gaussian prior mean value");
   return _parameters.at(param)->_gausMean;
}

double PhysParams::GaussianPriorSigma(const std::string& param) const
{
   StrictCheck(param, "Gaussian prior sigma value");
   return _parameters.at(param)->_gausSigma;
}

double PhysParams::FlatPriorMinimum(const std::string& param) const
{
   StrictCheck(param, "Flat prior minimum value");
   return _parameters.at(param)->_flatMinimum;
}

double PhysParams::FlatPriorMaximum(const std::string& param) const
{
   StrictCheck(param, "Flat prior maximum value");
   return _parameters.at(param)->_flatMaximum;
}

const std::string& PhysParams::UserPriorFilename(const std::string& param) const
{
   StrictCheck(param, "User prior histogram");
   return _parameters.at(param)->_userFilename;
}

bool PhysParams::IsFixed(const std::string& param) const
{
   StrictCheck(param, "Fixed");
   return _parameters.at(param)->_isFixed;
}

bool PhysParams::IsNuisance(const std::string& param) const
{
   StrictCheck(param, "Nuisance");
   return _parameters.at(param)->_isNuisance;
}

bool PhysParams::IsMigradErrorSet(const std::string& param) const
{
   StrictCheck(param, "Migrad error set");
   return _parameters.at(param)->_hasMigradError;
}

double PhysParams::MigradError(const std::string& param) const
{
   StrictCheck(param, "Migrad error");
   return _parameters.at(param)->_migradError;
}

bool PhysParams::IsHesseErrorSet(const std::string& param) const
{
   StrictCheck(param, "Hesse error set");
   return _parameters.at(param)->_hasHesseError;
}

double PhysParams::HesseError(const std::string& param) const
{
   StrictCheck(param, "Hesse error");
   return _parameters.at(param)->_hesseError;
}

bool PhysParams::IsMinosErrorSet(const std::string& param) const
{
   StrictCheck(param, "Minos error set");
   return _parameters.at(param)->_hasMinosError;
}

double PhysParams::MinosLowError(const std::string& param) const
{
   StrictCheck(param, "Minos low error");
   return _parameters.at(param)->_minosLowError;
}

double PhysParams::MinosHighError(const std::string& param) const
{
   StrictCheck(param, "Minos high error");
   return _parameters.at(param)->_minosHighError;
}

conv::PhysParamId_t PhysParams::GetParamId(const std::string& param) const
{
   // Check if the parameter exists
   auto item = _parameters.find(param);
   if(item != _parameters.end())
   {  // Parameter exists, return its ID
      return item->second->_id;
   }
   else
   {  // Parameter does not exist, return undefined
      return Undefined;
   }
}

conv::PhysParamId_t PhysParams::operator[](const std::string& param) const
{
   return GetParamId(param);
}

void PhysParams::AddParameter(const string& param)
{
   Param* p = new Param(param, static_cast<conv::PhysParamId_t>(_nextId));
   // Attempt to Insert the parameter
   auto value = _parameters.insert(std::make_pair(param, p));
   ++_nextId;
   // Check to see if the value was inserted (i.e. didn't already exist)
   if(not(value.second))
   {  // The element already exists and so was not inserted
      // Attempts to insert parameters already present suggests a problem in the
      // XML specification
      LOG("PhysParams", pERROR) << "Duplicate parameter: " << param;
      util::Die(-1);
   }
}

/*
This is the old processing code for handling the selected parameters for an
analysis. This should be moved to the FPL factory.*/
void PhysParams::ProcessParameters(const string& parameterList)
{
   vector<string> tokens = util::Tokens(parameterList, ",");
   for(auto token : tokens)
   {
      // Want to check for empty strings here, they are possible
      string param = util::TrimSpaces(token);
      if(not(token.empty()))
      {
         AddParameter(param);
      }
   }
}

void PhysParams::SetParameterValue(double& value, TXMLEngine* xml,
      XMLNodePointer_t node, const std::string& path, const double def)
{
   // Set parameter value, using default if necessary
   if(util::NodeExists(xml, node, path))
   {  // Value is specified, read it
      value = util::XML2Dbl(xml, node, path);
   }
   else
   {  // No value specified, use default
      value = def;
   }
}

void PhysParams::SetParameterValue(bool& value, TXMLEngine* xml,
      XMLNodePointer_t node, const std::string& path, const bool def)
{
   // Set parameter value, using default if necessary
   if(util::NodeExists(xml, node, path))
   {  // Value is specified, read it
      value = util::XML2Bool(xml, node, path);
   }
   else
   {  // No value specified, use default
      value = def;
   }
}

void PhysParams::SetParameterValue(std::string& value, TXMLEngine* xml,
      XMLNodePointer_t node, const std::string& path, const std::string& def)
{
   // Set parameter value, using default if necessary
   if(util::NodeExists(xml, node, path))
   {  // Value is specified, read it
      value = util::XML2String(xml, node, path);
   }
   else
   {  // No value specified, use default
      value = std::string(def);
   }
}

void PhysParams::ProcessParameter(TXMLEngine* xml, XMLNodePointer_t node)
{
   // Get the name attribute and create the parameter
   string name = xml->GetAttr(node, "name");
   AddParameter(name);

   // Set all of the parameter values, using defaults as necessary
   SetParameterValue(_parameters[name]->_bestfit, xml, node, "values/bestfit",
         _defaults->_bestfit);
   SetParameterValue(_parameters[name]->_initial, xml, node, "values/init",
         _defaults->_initial);
   SetParameterValue(_parameters[name]->_minimum, xml, node, "values/minimum",
         _defaults->_minimum);
   SetParameterValue(_parameters[name]->_maximum, xml, node, "values/maximum",
         _defaults->_maximum);
   SetParameterValue(_parameters[name]->_stepsize, xml, node, "values/step_size",
         _defaults->_stepsize);
   SetParameterValue(_parameters[name]->_scaling, xml, node, "values/scaling",
         _defaults->_scaling);
   SetParameterValue(_parameters[name]->_isMarginalised, xml, node,
         "marginalisation/set", _defaults->_isMarginalised);
   SetParameterValue(_parameters[name]->_gausMean, xml, node,
         "marginalisation/priors/gaus/mean", _defaults->_gausMean);
   SetParameterValue(_parameters[name]->_gausSigma, xml, node,
         "marginalisation/priors/gaus/sigma", _defaults->_gausSigma);
   SetParameterValue(_parameters[name]->_flatMinimum, xml, node,
         "marginalisation/priors/flat/minimum", _defaults->_flatMinimum);
   SetParameterValue(_parameters[name]->_flatMaximum, xml, node,
         "marginalisation/priors/flat/maximum", _defaults->_flatMaximum);
   SetParameterValue(_parameters[name]->_userFilename, xml, node,
         "marginalisation/priors/user/histogram", _defaults->_userFilename);
   SetParameterValue(_parameters[name]->_isFixed, xml, node,
         "properties/fixed", _defaults->_isFixed);
   SetParameterValue(_parameters[name]->_isNuisance, xml, node,
         "properties/fixed", _defaults->_isNuisance);
   SetParameterValue(_parameters[name]->_hasMigradError, xml, node,
         "errors/migrad/set", _defaults->_hasMigradError);
   SetParameterValue(_parameters[name]->_migradError, xml, node,
         "errors/migrad/error", _defaults->_migradError);
   SetParameterValue(_parameters[name]->_hasHesseError, xml, node,
         "errors/hesse/set", _defaults->_hasHesseError);
   SetParameterValue(_parameters[name]->_hesseError, xml, node,
         "errors/hesse/error", _defaults->_hesseError);
   SetParameterValue(_parameters[name]->_hasMinosError, xml, node,
         "errors/minos/set", _defaults->_hasMinosError);
   SetParameterValue(_parameters[name]->_minosLowError, xml, node,
         "errors/minos/low_error", _defaults->_minosLowError);
   SetParameterValue(_parameters[name]->_minosHighError, xml, node,
         "errors/minos/high_error", _defaults->_minosHighError);
}

void PhysParams::ProcessDefaults(TXMLEngine* xml, XMLNodePointer_t node)
{
   _defaults->_bestfit = util::XML2Dbl(xml, node, "values/bestfit");
   _defaults->_initial = util::XML2Dbl(xml, node, "values/init");
   _defaults->_minimum = util::XML2Dbl(xml, node, "values/minimum");
   _defaults->_maximum = util::XML2Dbl(xml, node, "values/maximum");
   _defaults->_stepsize = util::XML2Dbl(xml, node, "values/step_size");
   _defaults->_scaling = util::XML2Dbl(xml, node, "values/scaling");
   _defaults->_isMarginalised = util::XML2Bool(xml, node, "marginalisation/set");
   _defaults->_gausMean = util::XML2Dbl(xml, node,
         "marginalisation/priors/gaus/mean");
   _defaults->_gausSigma = util::XML2Dbl(xml, node,
         "marginalisation/priors/gaus/sigma");
   _defaults->_flatMinimum = util::XML2Dbl(xml, node,
         "marginalisation/priors/flat/minimum");
   _defaults->_flatMaximum = util::XML2Dbl(xml, node,
         "marginalisation/priors/flat/maximum");
   _defaults->_userFilename = util::XML2String(xml, node,
         "marginalisation/priors/user/histogram");
   _defaults->_isFixed = util::XML2Bool(xml, node, "properties/fixed");
   _defaults->_isNuisance = util::XML2Bool(xml, node, "properties/nuisance");
   _defaults->_hasMigradError = util::XML2Bool(xml, node, "errors/migrad/set");
   _defaults->_migradError = util::XML2Dbl(xml, node, "errors/migrad/error");
   _defaults->_hasHesseError = util::XML2Bool(xml, node, "errors/hesse/set");
   _defaults->_hesseError = util::XML2Dbl(xml, node, "errors/hesse/error");
   _defaults->_hasMinosError = util::XML2Bool(xml, node, "errors/minos/set");
   _defaults->_minosLowError = util::XML2Dbl(xml, node,
         "errors/minos/low_error");
   _defaults->_minosHighError = util::XML2Dbl(xml, node,
         "errors/minos/high_error");
}

void PhysParams::StrictCheck(const std::string& param, const std::string& value)
   const
{
   if(_parameters.find(param) == _parameters.end())
   {
      LOG("PhysParams", pERROR) << "Cannot get " << value << ". " <<
         "No such parameter: " << param;
      util::Die(-1);
   }
}

}
