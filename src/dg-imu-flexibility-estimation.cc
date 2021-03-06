#include <sstream>

#include <dynamic-graph/factory.h>
#include <dynamic-graph/command-setter.h>
#include <dynamic-graph/command-getter.h>
#include <dynamic-graph/command-bind.h>

#include <sot-state-observation/dg-imu-flexibility-estimation.hh>


namespace sotStateObservation
{
    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN ( DGIMUFlexibilityEstimation,
                                          "DGIMUFlexibilityEstimation" );

    DGIMUFlexibilityEstimation::DGIMUFlexibilityEstimation
                ( const std::string & inName):
        Entity(inName),
        measurementSIN(0x0 , "DGIMUFlexibilityEstimation("+inName+")::input(vector)::measurement"),
        inputSIN(0x0 , "DGIMUFlexibilityEstimation("+inName+")::input(vector)::input"),
        contactsNbrSIN(0x0 , "DGIMUFlexibilityEstimation("+inName+")::input(unsigned)::contactNbr"),
        contact1SIN(0x0, "DGIMUFlexibilityEstimation("+inName+")::input(vector)::contact1"),
        contact2SIN(0x0, "DGIMUFlexibilityEstimation("+inName+")::input(vector)::contact2"),
        contact3SIN(0x0, "DGIMUFlexibilityEstimation("+inName+")::input(vector)::contact3"),
        contact4SIN(0x0, "DGIMUFlexibilityEstimation("+inName+")::input(vector)::contact4"),
        flexibilitySOUT(0x0,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexibility"),
        flexPoseThetaUSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexPoseThetaU"),
        flexPositionSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexPosition"),
        flexVelocitySOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexVelocity"),
        flexAccelerationSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexAcceleration"),
        flexThetaUSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexThetaU"),
        flexTransformationMatrixSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(Matrix)::flexTransformationMatrix"),
        flexOmegaSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexOmega"),
        flexOmegaDotSOUT(flexibilitySOUT,
                        "DGIMUFlexibilityEstimation("+inName+")::output(vector)::flexOmegaDot")
    {

        signalRegistration (measurementSIN);
        signalRegistration (inputSIN);
        signalRegistration (flexibilitySOUT);

        signalRegistration (flexPoseThetaUSOUT);
        signalRegistration (flexPositionSOUT);
        signalRegistration (flexVelocitySOUT);
        signalRegistration (flexAccelerationSOUT);
        signalRegistration (flexThetaUSOUT);
        signalRegistration (flexTransformationMatrixSOUT);
        signalRegistration (flexOmegaSOUT);
        signalRegistration (flexOmegaDotSOUT);

        signalRegistration (contact1SIN);
        signalRegistration (contact2SIN);
        signalRegistration (contact3SIN);
        signalRegistration (contact4SIN);
        signalRegistration (contactsNbrSIN);



        dynamicgraph::Vector measure(measurementSize);
        dynamicgraph::Vector input(inputSize);
        dynamicgraph::Vector flexibility(stateSize);
        dynamicgraph::Vector flexPoseThetaU(6);
        dynamicgraph::Vector flexPosition(3);
        dynamicgraph::Vector flexVelocity(3);
        dynamicgraph::Vector flexAcceleration(3);
        dynamicgraph::Vector flexThetaU(3);
        dynamicgraph::Matrix flexTransformationMatrix(3,3);
        dynamicgraph::Vector flexOmega(3);
        dynamicgraph::Vector flexOmegaDot(3);
        dynamicgraph::Vector contactPosition(3);

        flexTransformationMatrix.setIdentity();


        measurementSIN.setConstant(measure);
        inputSIN.setConstant(input);
        flexibilitySOUT.setConstant(flexibility);
        flexPoseThetaUSOUT.setConstant(flexPoseThetaU);
        flexPositionSOUT.setConstant(flexPosition);
        flexVelocitySOUT.setConstant(flexVelocity);
        flexAccelerationSOUT.setConstant(flexAcceleration);
        flexThetaUSOUT.setConstant(flexThetaU);
        flexTransformationMatrixSOUT.setConstant(flexTransformationMatrix);
        flexOmegaSOUT.setConstant(flexOmega);
        flexOmegaDotSOUT.setConstant(flexOmegaDot);
        contactsNbrSIN.setConstant(0);
        contact1SIN.setConstant(contactPosition);
        contact2SIN.setConstant(contactPosition);
        contact3SIN.setConstant(contactPosition);
        contact4SIN.setConstant(contactPosition);

        flexibilitySOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexibility,
				    this, _1, _2));

        flexPoseThetaUSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexPoseThetaU,
				    this, _1, _2));

        flexPositionSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexPosition,
				    this, _1, _2));

        flexVelocitySOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexVelocity,
				    this, _1, _2));

        flexAccelerationSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexAcceleration,
				    this, _1, _2));

        flexThetaUSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexThetaU,
				    this, _1, _2));

        flexTransformationMatrixSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexTransformationMatrix,
				    this, _1, _2));

        flexOmegaSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexOmega,
				    this, _1, _2));

        flexOmegaDotSOUT.setFunction(boost::bind(&DGIMUFlexibilityEstimation::computeFlexOmegaDot,
				    this, _1, _2));

        std::ostringstream stateSizeString;
        stateSizeString << stateSize;

        std::ostringstream measurementSizeString;
        stateSizeString << measurementSize;

        std::ostringstream inputSizeString;
        inputSizeString << inputSize;

        std::string docstring;

        //setStateGuess
        docstring =
                "\n"
                "    Set a guess of the flexibility state  \n"
                "    takes a tuple of " + stateSizeString.str() + "\n"
                "    floating point mumbers as input \n"
                "\n";

        addCommand(std::string("setFlexibilityGuess"),
	     new
	     ::dynamicgraph::command::Setter <DGIMUFlexibilityEstimation,dynamicgraph::Vector>
                (*this, &DGIMUFlexibilityEstimation::setFlexibilityGuess, docstring));

         //setStateGuessCovariance
        docstring =
                "\n"
                "    Set the covariance matrix of the current flexibility estimation \n"
                "    takes " + stateSizeString.str() + " tuples of" + stateSizeString.str() + "\n"
                "    floating point mumbers as input \n"
                "\n";

        addCommand(std::string("setFlexibilityGuessCovariance"),
	     new
	     ::dynamicgraph::command::Setter <DGIMUFlexibilityEstimation,dynamicgraph::Matrix>
                (*this, &DGIMUFlexibilityEstimation::setFlexibilityGuessCovariance, docstring));

         //setSamplingPeriod
        docstring =
                "\n"
                "    Set the sampling period of the system \n"
                "    takes a floating point mumber as input \n"
                "\n";

        addCommand(std::string("setSamplingPeriod"),
	     new
	     ::dynamicgraph::command::Setter <DGIMUFlexibilityEstimation,double>
                (*this, &DGIMUFlexibilityEstimation::setSamplingPeriod, docstring));

        //increment
        docstring  =
                "\n"
                "    Increments the time index of the output signal \n"
                "    takes no argument \n"
                "\n";

        addCommand(std::string("increment"),
                    ::dynamicgraph::command::makeCommandVoid0(*this, & DGIMUFlexibilityEstimation::increment ,
    					docstring));

        //increment
        docstring  =
                "\n"
                "    Gets the time index of the flexibility estimation \n"
                "\n";

        addCommand(std::string("getFlexTime"),
                    new ::dynamicgraph::command::Getter <DGIMUFlexibilityEstimation,int>
                    (*this, & DGIMUFlexibilityEstimation::getFlexTime ,docstring));


    }

    DGIMUFlexibilityEstimation::~DGIMUFlexibilityEstimation()
    {
    }

    dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexibility
                  (dynamicgraph::Vector & flexibility, const int& inTime)
    {
        //std::cout << "computeFlexibility " << inTime << std::endl;

        const dynamicgraph::Vector & measurement = measurementSIN(inTime);
        const dynamicgraph::Vector & input = inputSIN(inTime);
        const unsigned & contactNb = contactsNbrSIN(inTime);

        estimator_.setContactsNumber(contactNb);

        if (contactNb>0)
        {
            estimator_.setContactPosition(0,convertVector<stateObservation::Vector>(contact1SIN(inTime)));

            if (contactNb>1)
            {
                estimator_.setContactPosition(1,convertVector<stateObservation::Vector>(contact4SIN(inTime)));

                if (contactNb>2)
                {
                    estimator_.setContactPosition(2,convertVector<stateObservation::Vector>(contact3SIN(inTime)));

                    if (contactNb==4)
                    {
                        estimator_.setContactPosition(3,convertVector<stateObservation::Vector>(contact4SIN(inTime)));
                    }
                }
            }
        }

        estimator_.setMeasurement(convertVector<stateObservation::Vector>(measurement));
        estimator_.setMeasurementInput(convertVector<stateObservation::Vector>(input));

        flexibility = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector());

        return flexibility;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexPosition
                        (::dynamicgraph::Vector & flexibilityPosition, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityPosition = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().head(3));

        return flexibilityPosition;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexPoseThetaU
                        (::dynamicgraph::Vector & flexibilityPoseThetaU, const int& inTime)
    {
        //std::cout << "computeFlexPoseThetaU " << inTime << std::endl;

        flexibilitySOUT(inTime);

        stateObservation::Vector v = stateObservation::Vector::Zero(6,1);
        v.head(3) = estimator_.getFlexibilityVector().head(3);
        v.tail(3) = estimator_.getFlexibilityVector().segment(9,3);

        flexibilityPoseThetaU = convertVector<dynamicgraph::Vector>(v);

        return flexibilityPoseThetaU;
    }


    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexVelocity
                        (::dynamicgraph::Vector & flexibilityVelocity, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityVelocity = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().segment(3,3));

        return flexibilityVelocity;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexAcceleration
                        (::dynamicgraph::Vector & flexibilityAcceleration, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityAcceleration = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().segment(6,3));

        return flexibilityAcceleration;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexThetaU
                        (::dynamicgraph::Vector & flexibilityThetaU, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityThetaU = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().segment(9,3));

        return flexibilityThetaU;
    }

    ::dynamicgraph::Matrix& DGIMUFlexibilityEstimation::computeFlexTransformationMatrix
                        (::dynamicgraph::Matrix & flexibilityTransformationMatrix, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityTransformationMatrix = convertMatrix<dynamicgraph::Matrix>(estimator_.getFlexibility());

        return flexibilityTransformationMatrix;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexOmega
                        (::dynamicgraph::Vector & flexibilityOmega, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityOmega = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().segment(12,3));

        return flexibilityOmega;
    }

    ::dynamicgraph::Vector& DGIMUFlexibilityEstimation::computeFlexOmegaDot
                        (::dynamicgraph::Vector & flexibilityOmegaDot, const int& inTime)
    {
        flexibilitySOUT(inTime);

        flexibilityOmegaDot = convertVector<dynamicgraph::Vector>(estimator_.getFlexibilityVector().segment(15,3));

        return flexibilityOmegaDot;
    }
}
