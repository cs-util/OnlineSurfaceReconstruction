/*
	This file is part of the implementation for the technical paper

		Field-Aligned Online Surface Reconstruction
		Nico Schertler, Marco Tarini, Wenzel Jakob, Misha Kazhdan, Stefan Gumhold, Daniele Panozzo
		ACM TOG 36, 4, July 2017 (Proceedings of SIGGRAPH 2017)

	Use of this source code is granted via a BSD-style license, which can be found
	in License.txt in the repository root.

	@author Nico Schertler
*/

#pragma once

#include <vector>

#include "osr/Scan.h"
#include "osr/HierarchyDef.h"
#include "osr/Optimizer.h"
#include "osr/MeshSettings.h"
#include "osr/ExtractedMesh.h"
#include "osr/OSRLibrary.h"

#include <nsessentials/util/Observer.h>

namespace osr
{
	//Holds all relevant data of the current session
	class OSR_EXPORT DataBase
	{
	public:			
		MeshSettings meshSettings;
		Optimizer optimizer;
		std::vector<Scan*> scans;
		THierarchy hierarchy;

		//This event is raised whenever a new scan is added to this data object.
		nse::util::Observer<Scan*> ScanAdded;

		//This event is raised whenever a scan is removed from this data object, possibly due to integration of the scan.
		nse::util::Observer<Scan*> ScanRemoved;

		DataBase(ExtractedMesh& extractedMesh);

		void AddScan(Scan*);
		void RemoveScan(Scan*);

		//Registers the scan with respect to the hierarchy by modifying the scan's transform. The hierarchy must not be empty for this to work.
		void RegisterScan(Scan*);
		//Sets the transform of the scan that has been registered last to its old state.
		void UndoLastRegistration();

		//Resets all data and produces a clean data object.
		void Reset();

		//Integrates the specified scan into the hierarchy and removes it from this data object.
		void IntegrateScan(Scan*);

		//Saves the current state of this data object to a file.
		void saveToFile(const std::string& path) const;
		//Restores the state for this data object from a file.
		void loadFromFile(const std::string& path);

	protected:
		virtual ExtractedMesh* getExtractedMesh() = 0;
		virtual const ExtractedMesh* getExtractedMesh() const = 0;

	private:
		//For procedural color generation; scaling between spatial space and the domain of the color-generating function
		float colorScale;
		//Generates a procedural color for a given position. The color function is continuous over positions.
		Vector3us generateColor(const Vector3f& p);

		//Stores information that are necessary to revert a registration.
		Scan* lastRegistrationScan = nullptr;
		Eigen::Affine3f lastRegistration;		
	};

	//Specialization of the DataBase class that holds an un-renderable final mesh.
	class OSR_EXPORT Data : public DataBase
	{
	public:
		ExtractedMesh extractedMesh;

		Data();

	protected:
		ExtractedMesh * getExtractedMesh();
		const ExtractedMesh* getExtractedMesh() const;
	};
}